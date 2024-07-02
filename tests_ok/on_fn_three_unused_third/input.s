global define_type

global define
global get_globals_size
global init_globals

global on_fns

global on_a
global on_b

section .data

define_type: db "j", 0

on_fns:
	dq on_a
	dq on_b
	dq 0

section .text

extern define_j

define:
	call define_j wrt ..plt
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
