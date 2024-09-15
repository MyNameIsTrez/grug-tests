section .data

global define_type
define_type: db "h", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq 0

global strings
strings:

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_block_mask

extern game_fn_define_h

global define
define:
	mov eax, 42
	mov rdi, rax
	call game_fn_define_h wrt ..plt
	ret

global init_globals
init_globals:
	ret
