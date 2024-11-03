section .data

global define_type
define_type: db "a", 0

align 8
global globals_size
globals_size: dq 8

global strings
strings:

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern game_fn_define_a

global define
define:
	call game_fn_define_a wrt ..plt
	ret

global init_globals
init_globals:
	mov eax, 420
	mov rdi[byte 0x0], eax
	mov eax, 1337
	mov rdi[0x4], eax
	ret