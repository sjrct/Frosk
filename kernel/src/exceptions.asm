;
; kernel/src/exceptions.asm
;

[bits 64]
[default rel]

%include "common.inc"

extern reg_int

[section .text]

global setup_exceptions
setup_exceptions:
	mov rdi, 13
	mov rsi, excp_gp
	call reg_int
	ret

excp_gp:
	mov ax, KERN_DS
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov ss, ax
	
	pop rax

	xor rsi, rsi
	mov rsi, rax
	mov rdi, fmt_str

	extern kprintf
	call kprintf

	cli
.lp:
	hlt
	jmp .lp
	;iretq

[section .data]

fmt_str: db "General Protection Fault: Error Code = %d", 10, 0
