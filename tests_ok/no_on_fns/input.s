global define_type
global globals_size

global define
global init_globals

global on_fns

section .data

define_type: db "a", 0

globals_size: dq 0

on_fns:

section .text

extern define_a

define:
	call define_a wrt ..plt
	ret

init_globals:
	ret
