#include "asm.h"
#include "mmu.h"

.code 16
.global start
start:
	cli										# clear interrupt flag

	# zero segment registers
	xor		ax, ax
	mov		ds, ax
	mov 	es, ax
	mov 	ss, ax

	# handle A20
seta20.1:
	in		al, 0x64
	test 	al, 0x2
	jnz		seta20.1
	mov		al, 0xd1
	out		0x64, al

seta20.2
	in		al, 0x64
	test 	al, 0x2
	jnz		seta20.2
	mov		al, 0xdf
	out		0x60, al

	# swith from real mode to protected mode
	lgdt	gdtdesc
	mov		eax, cr0
	or		eax, CR0_PE
	mov		cr0, eax

	# complete the transition to 32-its protected mode
	ljmp	start32, SEG_KCODE<<3

.code32										# tell assembler generate 32-bit code from now on
start32:
	# set up registers and call into C
	mov		ax, SEG_KDATA<<3
	mov		ds, ax
	mov		es, ax
	mov		ss, ax
	mov 	ax, 0
	mov		fs, ax
	mov 	gs, ax
	mov		esp, start
	call	bootmain

spin:
	jmp spin

# bootstrap GDT
.p2align 2
gdt:
	SEG_NULLASM								# null segment
	SEG_ASM(STA_X|STA_R, 0x, 0xffffffff)	# code segment
	SEG_ASM(STA_W, 0x0, 0xffffffff)			# data segment

gdtdesc:
	.word	(gdtdesc - gdt - 1)
	.long	gdt
