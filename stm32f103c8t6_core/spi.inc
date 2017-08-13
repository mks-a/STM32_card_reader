SPI_DEF = 0

@ initialize comunication through SPI2 pins
@ function does not take, nor recieve parameters
spi_init_spi2:
	push {r0, r1}	
	
	@ enable SPI2 clock
	ldr r1, =RCC_APB1ENR
	ldr r0, [r1]
	
	orr r0, r0, 0x4000
	str r0, [r1]
	
	@ set CPHA (bit 0) = 1, CPOL (bit 1) = 0, BR[2:0] (bits 5:3) = 000, DFF (bit 11) = 1
	ldr r1, =SPI2_CR1
	ldr r0, [r1]
	
	orr r0, r0, 0x801
	str r0, [r1]
	
	pop {r0, r1}
	bx lr
	
@ enable communication through SPI2 pins
@ function does not take, nor recieve parameters
spi_enable_spi2:
	push {r0, r1}
	
	@ set MSTR (bit 2) = 1, SPE (bit 7) = 1
	ldr r1, =SPI2_CR1
	ldr r0, [r1]
	
	orr r0, r0, 0x84
	
	str r0, [r1]

	pop {r0, r1}
	bx lr
