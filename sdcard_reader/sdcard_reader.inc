SDCARD_READER_DEF = 0

.ifndef SPI_DEF
.include "stm32f103c8t6_core/spi.inc"
.endif

.section .text
@ initialize sdcard reader for using
@ function does not take, nor recieve parameters
@ Every command byte sent to an SD must have bit 6 set (0x40) (so the device can recognise it as a command and not some data).
@ But every time we send a command (and it's parameters), we have to wait for a not-busy response from the SD card. 
@ While the SD card is busy, it holds its "output pin" high - the data clocked out of it is always 11111111
sdcard_reader_spi2_init:
	push {r0, r1, r2, lr}
	
	@ clear last_response value
	eor r0, r0, r0			@clear r0 register
	ldr r1, =last_response	@ load address of last_response to r1
	str r0, [r1]
	
	bl spi_init_spi2
	bl spi_master_enable_spi2
	
	@ call send function
	@ set data to send (hope that is is CMD0)
	ldr r0, =0x40		@ CMD0
	bl spi_send_data8_sync_spi2
	
	ldr r0, =last_response
	bl spi_read_sync_response_spi2
	
@ send 4 parameter bytes (all zeroes for CMD0)
	ldr r0, =0x00
	ldr r1, =0x04		@ set loop counter to 4

_sdcard_reader_spi2_init_send_cmd0_param:
	bl spi_send_data8_sync_spi2
	
	push {r0}
	ldr r0, =last_response
	bl spi_read_sync_response_spi2
	
	ldr r2, =data_readed
	
	strh r0, [r2]		@ store half-word to data_readed
	pop {r0}
	
	subs r1, r1, 0x01
	bne _sdcard_reader_spi2_init_send_cmd0_param	@ branch if not equal
	
	bkpt
	ldr r1, =last_response
	ldr r0, [r1]									@ TODO: how to access memory location directly?
	ldr r1, =data_readed
	ldrh r2, [r1]									@ load half-word from SRAM
	
	pop {r0, r1, r2, lr}
	bx lr
	
@ sends command to SD card via SPI interface
@ recieve command in r0 register
@ depending on selected mode 8 or 16 bit would be sended
@ do not return any value
sdcard_send_spi2_cmd:
	push {r0, r1}
	
	ldr r1, =SPI2_DR
	str r0, [r1]
	
	pop {r0, r1}
	bx lr
	
.section .bss
@ .lcomm defines a link and its lenght (it is useful in bss section, because I will not initialize data (.word 0x04 will initialize data, so I can't use it in bss))
.lcomm last_response, 4
.lcomm data_readed, 2
	