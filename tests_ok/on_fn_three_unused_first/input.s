global define_type

global define
global get_globals_size
global init_globals

global on_fns

global on_b
global on_c

section .data

define_type: db "j", 0

on_fns:
	dq 0
	dq on_b
	dq on_c

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

on_b:
	ret

on_c:
	ret
