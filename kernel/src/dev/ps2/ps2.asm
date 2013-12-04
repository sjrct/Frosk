;
; ps2.asm
;

[bits 64]
[default rel]

extern ps2_kb_put

[section .text]

%include "stack.inc"

global ps2_kb_int
ps2_kb_int:
	SAVE_SEGMENTS
	PUSH_CALLEE
	PUSH_CALLER

	mov eax, [composed]
	in al, 0x60

	cmp al, 0xf0
	jne .not_key_up

	shl eax, 8
	mov [composed], eax
	jmp .key_up

.not_key_up:
	mov dword [composed], 0
	xor rdi, rdi
	mov edi, eax
	call ps2_kb_put

.key_up:
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

[section .data]

composed: dd 0
