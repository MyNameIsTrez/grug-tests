global define_type
global globals_size

global define
global init_globals

global on_fns

global on_a
global on_c

section .data

define_type: db "j", 0

globals_size: dq 0

on_fns:
	dq on_a
	dq 0
	dq on_c

section .text

extern define_j

define:
	call define_j wrt ..plt
	ret

init_globals:
	ret

on_a:
	ret

on_c:
	ret
