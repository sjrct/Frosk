;
; kernel/src/dev/dev.asm
;

[bits 64]
[default rel]

%include "stack.inc"

extern devices
extern handle_lookup

; reflected in kernel/src/dev/dev.h
struc device_t
	.type:       resd 1
	.hndl:       resd 1
	.block_size: resd 1
	.enabled:    resd 1
	.data:       resq 1
	.enable:     resq 1
	.disable:    resq 1
	.functions:  resq 11
endstruc

[section .text]

global devcall
devcall:
	push rsi
	push rdx
	push rcx
	push r8
	push r9

	mov rsi, rdi
	mov rdi, [devices]
	call handle_lookup

	mov rdi, rax
	pop r8
	pop rcx
	pop rdx
	pop rsi
	pop r11

	shl r11, 3
	add rax, r11
	mov rax, [rax + device_t.functions]

	; TODO throw error when null function
	test rax, rax
	jz .skip
	call rax
.skip:

	ret

[section .rdata]

fmt: db '~~~~~%d', 10, 0
