section .data

global define_type
define_type: db "q", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "a", 0
	db "b", 0

section .text

extern game_fn_define_q

global define
define:
	lea rdi, strings[rel 0]
	lea rsi, strings[rel 2]
	lea rdx, strings[rel 2]
	call game_fn_define_q wrt ..plt
	ret

global init_globals
init_globals:
	ret
