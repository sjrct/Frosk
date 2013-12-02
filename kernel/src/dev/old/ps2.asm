;
; ps2.asm
;

[bits 64]
[default rel]

[section .text]

%include "stack.inc"

global ps2_kb_int
ps2_kb_int:
	PUSH_CALLER

	xor rax, rax
	in al, 0x60

	extern keyboard_stream
	extern sputc
	mov rdi, rax
	mov rsi, keyboard_stream
	call sputc

	mov al, 0x20
	out 0x20, al

	POP_CALLER
	iretq


global ps2_ms_int
ps2_ms_int:
	PUSH_CALLER

	mov ax, 0x20
	out 0x20, ax
	
	POP_CALLER
	iretq
