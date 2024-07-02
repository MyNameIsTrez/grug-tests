global define_type

global define
global get_globals_size
global init_globals

global on_fns

section .data

define_type: db "d", 0

on_fns:
	dq 0

section .text

extern define_d

define:
	call define_d wrt ..plt
	ret

get_globals_size:
	mov eax, 0
	ret

init_globals:
	ret
