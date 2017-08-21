.thumb
@This turns on Unified Assembly Language which is required to get all the features of Thumb-2.
.syntax unified					@ without this line "orr r0, r0, #0x200000" will give an error "unshifted register required" 
.cpu cortex-m3

.equ STACKINIT, 0x20005000		@ end of SRAM 20kb

.section .text
.org 0x00000000					@ this not work!!!
SP: .word STACKINIT				@ 20kb of RAM, so I will put stack pointer to the end of it
RESET: .word main
NMI_HANDLER: .word nmi_fault
HARD_FAULT: .word hard_fault
MEMORY_FAULT: .word memory_fault
BUS_FAULT: .word bus_fault
USAGE_FAULT: .word usage_fault
.org 0x000000D0
SPI2_INTERRUPT: .word spi2_interrupt + 1

.ifndef SDCARD_DEF
.include "sdcard_reader/sdcard_reader.inc"
.endif

@.balign 2 				@ if bit 0 of address is 1 this indicate Thumb state of CPU, for Cortex-M it always should be 1, becaus it not support ARM state
main:
	
	@ TODO: temporary disable SPI interrupt 
	cpsie i						@ enable interrupts
	push {lr}
	bl sdcard_reader_spi2_init
	pop {lr}
	
_main_loop:

	wfi 						@ wait for interrupt
	
	b _main_loop				@ branch to _main_loop and not load return address to link register (LR)
	
	@ return from function
	bx lr						@ indirect branch to link register address

nmi_fault:
	@ breakpoint
	bkpt
	bx lr
	
hard_fault:
	@ breakpoint
	bkpt
	bx lr

memory_fault:
	@ breakpoint
	bkpt
	bx lr

bus_fault:
	@ breakpoint
	bkpt
	bx lr

usage_fault:
	@ breakpoint
	bkpt
	bx lr
	
spi2_interrupt:
	bx lr

@ this is dummy function that just return from interrupt
returtn_form_interrupt:
	@ breakpoint
	bkpt
	bx lr
	
.end
	