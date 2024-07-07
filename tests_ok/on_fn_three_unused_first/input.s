section .data

global define_type
define_type: db "j", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq 0
	dq on_b
	dq on_c

section .text

extern define_j

global define
define:
	call define_j wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_b
on_b:
	ret

global on_c
on_c:
	ret
