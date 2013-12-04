;
; syscalls_ls.asm
;
; written by sjrct
;

[section .rodata]

%macro FUNC 1
	extern %1
	dq %1
%endmacro

syscalls_ls:
	FUNC do_i_live
	FUNC fexec_sc
	FUNC dev_of
	FUNC get_dev_type
	FUNC devcall
	FUNC poll_port
	FUNC read_port
	FUNC write_port
	FUNC refuse_port
	FUNC get_state
	dq 0

syscalls_count: equ ($ - syscalls_ls) / 8
