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

extern define_q

global define
define:
	lea rdi, [rel strings+0]
	lea rsi, [rel strings+2]
	lea rdx, [rel strings+2]
	call define_q wrt ..plt
	ret

global init_globals
init_globals:
	ret