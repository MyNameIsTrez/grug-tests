global define_type
global globals_size

global define
global init_globals

global on_fns

global on_a
global on_b

section .data

define_type: db "e", 0

align 8
globals_size: dq 0

on_fns:
	dq on_a
	dq on_b

section .text

extern define_e

define:
	call define_e wrt ..plt
	ret

init_globals:
	ret

on_a:
	ret

on_b:
	ret
