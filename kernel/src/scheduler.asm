;
; kernel/src/scheduler.asm
;

[bits 64]
[default rel]

%include "stack.inc"
%include "common.inc"

extern set_irq
extern swapflop
extern scheduler_lock
extern head_thrd
extern head_proc

; reflected in include/kernel/scheduler.h
struc process
	.next:      resq 1
	.first:     resq 1
	.code:      resq 1
	.timeslice: resd 1
	.level:     resd 1
	.main:      resq 1
	.argv:      resq 1
	.argc:      resd 1
endstruc

struc thread
	.parent:      resq 1
	.next_inproc: resq 1
	.next_sched:  resq 1
	.stack:       resq 1
	.saved_rsp:   resq 1
	.state:       resd 1
	.pack:        resd 1
endstruc

[section .text]

; this function handles task switching
global timer_irq
timer_irq:
	PUSH_CALLER
	PUSH_CALLEE
	SAVE_SEGMENTS
	push rbx

	; check locking
	test byte [scheduler_lock], 1
	jz .return_early

	; find ready thread
	mov rbx, [abs head_thrd]
	test rbx, rbx
	jz .return_early
	mov rax, rbx

.search_loop:
	cmp byte [rbx + thread.state], 0
	je .search_loop_done
	
	mov rbx, [rbx + thread.next_sched]
	cmp rax, rbx
	je .return_early
	jmp .search_loop
.search_loop_done:

	; update state/next thread
	mov byte [rbx + thread.state], 2
	mov rax, [rbx + thread.next_sched]
	mov [head_thrd], rax
	
	; swap in code pages
	xor rdi, rdi
	mov rcx, [cur_thrd]
	test rcx, rcx
	jz .ct_null

	mov [rcx + thread.parent], rcx
	mov [rcx + process.code], rdi
.ct_null:

	mov rsi, [rbx + thread.parent]
	mov rsi, [rsi + process.code]
	mov rdx, 0x7
	call swapflop

	; push iretq data
	mov rax, rsp
	push qword USER_DS | 3
	push rax
	pushfq
	push qword USER_CS | 3
	mov rax, .return_here
	push rax

	; change old thread status
	xor rdi, rdi
	mov rax, [cur_thrd]
	test rax, rax
	jz .ct_null2

	mov [rax + thread.stack], rdi
	mov byte [rax + thread.state], 0
	mov [rax + thread.saved_rsp], rsp
.ct_null2:
	mov [cur_thrd], rbx
	
	; swap in stack
	mov rsi, [rbx + thread.stack]
	mov rdx, 0x7
	call swapflop

	; load rsp
	mov rsp, [rbx + thread.saved_rsp]

	; interrupt finished/timer reset
	mov al, 0x20
	out 0x20, al

	mov al, 0xff
	out 0x40, al
	out 0x40, al

	;mov qword [rsp], .return_here
	or qword [rsp + 0x10], 0x200
	iretq
.return_here:
	cli

.return:
	pop rbx
	RESTORE_SEGMENTS
	POP_CALLEE
	POP_CALLER
	iretq

.return_early:
	mov al, 0x20
	out 0x20, al

	mov al, 0xff
	out 0x40, al
	out 0x40, al
	jmp .return


[section .data]

global preamble_size
global preamble_code
preamble_size: equ preamble_code.end - preamble_code
preamble_code:
	mov ax, USER_DS | 3
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	pop rdi
	pop rsi
	pop rax
	
	call rax

	; TODO System call to end thread
	mov r15, 0xc0de15dead
	jmp $
.end:

align 8

global cur_thrd
cur_thrd: dq 0
