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
	mov edi, 1
	mov esi, 2
	mov edx, 3
	mov ecx, 4
	mov r8d, 5
	mov r9d, 6
	call game_fn_define_n wrt ..plt
	ret

global init_globals
init_globals:
	ret
