global define_type
global globals_size

global define
global init_globals

global on_fns

section .data

define_type: db "d", 0

globals_size: dq 0

on_fns:
	dq 0

section .text

extern define_d

define:
	call define_d wrt ..plt
	ret

init_globals:
	ret
