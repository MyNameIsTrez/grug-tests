section .data

global define_type
define_type: db "a", 0

align 8
global globals_size
globals_size: dq 0

section .text

extern grug_block_mask

extern game_fn_define_a

global define
define:
	call game_fn_define_a wrt ..plt
	ret

global init_globals
init_globals:
	ret
