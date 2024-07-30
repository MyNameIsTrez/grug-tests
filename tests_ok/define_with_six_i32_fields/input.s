section .data

global define_type
define_type: db "n", 0

align 8
global globals_size
globals_size: dq 0

section .text

extern game_fn_define_n

global define
define:
	mov rdi, 1
	mov rsi, 2
	mov rdx, 3
	mov rcx, 4
	mov r8, 5
	mov r9, 6
	call game_fn_define_n wrt ..plt
	ret

global init_globals
init_globals:
	ret
