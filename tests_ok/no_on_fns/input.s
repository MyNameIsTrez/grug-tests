section .data

global define_type
define_type: db "a", 0

align 8
global globals_size
globals_size: dq 0

section .text

extern define_a

global define
define:
	call define_a wrt ..plt
	ret

global init_globals
init_globals:
	ret
