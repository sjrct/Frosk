;
; ps2.asm
;

[bits 64]
[default rel]

[section .text]

%include "stack.inc"

global ps2_kb_int
ps2_kb_int:
	SAVE_SEGMENTS
	PUSH_CALLEE
	PUSH_CALLER

	xor rax, rax
	in al, 0x60

	mov rdi, rax
	extern ps2_kb_put
	call ps2_kb_put

	mov al, 0x20
	out 0x20, al

	POP_CALLER
	POP_CALLEE
	RESTORE_SEGMENTS
	iretq


;global ps2_ms_int
;ps2_ms_int:
;	PUSH_CALLER

;	mov ax, 0x20
;	out 0x20, ax

;	POP_CALLER
;	iretq
