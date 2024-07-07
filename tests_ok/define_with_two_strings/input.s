global define_type
global globals_size
global strings

global define
global init_globals

section .data

define_type: db "l", 0

globals_size: dq 0

strings:
	db "military", 0
	db "foo", 0

section .text

extern define_l

define:
	lea rdi, [rel strings+0]
	lea rsi, [rel strings+9]
	call define_l wrt ..plt
	ret

init_globals:
	ret
