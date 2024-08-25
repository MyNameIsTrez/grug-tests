section .data

global define_type
define_type: db "a", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:

section .text

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_block_mask

extern game_fn_define_a

global define
define:
	call game_fn_define_a wrt ..plt
	ret

global init_globals
init_globals:
	ret
