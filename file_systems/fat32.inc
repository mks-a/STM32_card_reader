FAT32_DEF = 0

@ reads data from file
@ return value in r12 register
fat32_read_file:
	bx lr
	
@ writes data to file
@ value to write is passed through r0 register
@ function does not return parameters
fat32_write_file:
	bx lr

fat32_open_file:
	bx lr
	
fat32_close_file:
	bx lr
	
fat32_create_file:
	bx lr

fat32_delete_file:
	bx lr
	