global define_type

global define
global get_globals_size
global init_globals

global on_fns

global on_a
global on_b

section .data

define_type: db "e", 0

on_fns:
	dq on_a
	dq on_b

section .text

extern define_e

define:
	call define_e wrt ..plt
	ret

get_globals_size:
	mov eax, 0
	ret

init_globals:
	ret

on_a:
	ret

on_b:
	ret
