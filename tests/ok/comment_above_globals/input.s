section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 12

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

extern game_fn_define_d

global define
define:
	call game_fn_define_d wrt ..plt
	ret

global init_globals
init_globals:
	mov eax, 420
	mov rdi[byte 0x0], eax

	mov eax, 1337
	mov rdi[0x4], eax

	mov eax, 100
	mov rdi[0x8], eax

	ret
