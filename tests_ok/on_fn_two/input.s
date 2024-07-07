section .data

global define_type
define_type: db "e", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a
	dq on_b

section .text

extern define_e

global define
define:
	call define_e wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
	ret

global on_b
on_b:
	ret
