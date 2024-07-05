global define_type
global globals_size

global define
global init_globals

global on_fns

global on_a
global on_b

section .data

define_type: db "j", 0

globals_size: dq 0

on_fns:
	dq on_a
	dq on_b
	dq 0

section .text

extern define_j

define:
	call define_j wrt ..plt
	ret

init_globals:
	ret

on_a:
	ret

on_b:
	ret
