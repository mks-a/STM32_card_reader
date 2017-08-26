SPI_DEF = 0

.ifndef STM32F103C8T6_REGISTERS
.include "stm32f103c8t6_core/registers.inc"
.endif

.section .text
@ initialize comunication through SPI2 pins
@ function does not take, nor recieve parameters
spi_init_spi2:
	push {r0, r1}	

	@ enable SPI2 clock
	ldr r1, =RCC_APB1ENR
	ldr r0, [r1]
	
	orr r0, r0, 0x4000
	str r0, [r1]
	
	@ set MSTR (bit 2) = 1, LSBFIRST (bit 7) = 1, SSI (bit 8) = 1, SSM (bit 9) = 1 
	ldr r1, =SPI2_CR1	
	ldr r0, =0x380
	str r0, [r1]
	
	pop {r0, r1}
	bx lr
	
@ enable communication through SPI2 pins
@ function does not take, nor recieve parameters
spi_master_enable_spi2:
	push {r0, r1}
	
	@ SPE (bit 6) = 1
	ldr r1, =SPI2_CR1
	ldr r0, [r1]
	
	@MSTR and SPE bits must be set (they remain set only if the NSS pin is connected to a high-level signal)
	orr r0, r0, 0x44
	str r0, [r1]

	pop {r0, r1}
	bx lr
	
@ sends 8 bit data to SPI2
@ recieve 8 bit data in r0 register
@ function does not return any value
@ this function sends data syncronuously (do not return control to the main program until data will be send)
spi_send_data8_sync_spi2:
	push {r0, r1, r2}		@ pushing executed so r2, r1, r0
	
	@ save base pointer
	mov r2, sp
	
	ldr r1, =SPI2_DR
	ldr r0, [r2]			@ load passed value (r0) from the stack
	@ clear 3 highest bits in r0 register
	and r0, r0, 0xFF
	
	str r0, [r1]
	
@ loop while TXE (bit 1) in SPI_SR is 0 (transmit buffer not empty)
_spi_send_data8_sync_spi2_wait_loop:
	ldr r1, =SPI2_SR
	ldr r0, [r1]
	ands r0, r0, 0x02									@ check for 1 bit (does it set or not)
	beq	_spi_send_data8_sync_spi2_wait_loop				@ branch if zero
	pop {r0, r1, r2}	@ poping executed so r0, r1, r2
	bx lr

@ reads response from SPI2 data register and clear RXNE flag in SPI2_SR register
@ function recieve in r0 register a pointer to location where SPI2_DR should be stored
@ function return true in r0 register if data was recieved, otherwise false
spi_read_sync_response_spi2:
	push {r0, r1, r2, r3, lr}
	
	@ save base pointer
	mov r3, sp
	
	@ Check for RXNE bit (bit 0) in SPI2_SR register
	eor r2, r2, r2
	ldr r1, =SPI2_SR
	ldr r0, [r1]
	ands r0, r0, 0x01					
	beq _spi_read_sync_response_spi2_exit	@ exit if RXNE not set (r0 will be 0x00 if its jump to exit)
	
	ldr r1, =SPI2_SR
	ldr r0, [r1]
	ldr r2, =0xFFFE				
	and r0, r0, r2
	str r0, [r1]
	
	eor r2, r2, r2				@ clear register
	ldr r1, =SPI2_DR
	ldr r2, [r1]
	
	@ load pointer from the stack to r0 register
	ldr r0, [r3]
	
	str r2, [r0]				@ save value from data register to location from r0
	
	ldr r0, =0x01				@ return true
	
_spi_read_sync_response_spi2_exit:
	pop {r1}				@ r1 register pop twise, because I shouldn't overwrite r0 value, hope that r1 register will get correct value
	pop {r1, r2, r3, lr}
	bx lr	
