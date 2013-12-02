;
; lock.asm
;

[bits 64]
[default rel]

[section .text]

global trylock
trylock:
	xor rax, rax
	lock xchg byte [rdi], al
	ret

global getlock
getlock:
	call trylock
	test al, al
	jz getlock
	ret

global unlock
unlock:
	mov byte [rdi], 1
	ret
