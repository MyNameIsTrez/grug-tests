section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq 0

section .text

extern game_fn_define_d

global define
define:
	call game_fn_define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret
