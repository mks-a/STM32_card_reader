SDCARD_READER_DEF = 0

.ifndef SPI_DEF
.include "stm32f103c8t6_core/spi.inc"
.endif

@ initialize sdcard reader for using
@ function does not take, nor recieve parameters
sdcard_reader_init:
	push {lr}
	bl spi_init_spi2
	bl spi_enable_spi2
	pop {lr}

	bx lr
	
@ reads data from file
@ return value in r12 register
sdcard_reader_read_file:
	bx lr
	
@ writes data to file
@ value to write is passed through r0 register
@ function does not return parameters
sdcard_reader_write_file:
	bx lr

sdcard_reader_open_file:
	bx lr
	
sdcard_reader_close_file:
	bx lr
	
sdcard_reader_create_file:
	bx lr

sdcard_reader_delete_file:
	bx lr