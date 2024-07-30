section .data

global define_type
define_type: db "h", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq 0

section .text

extern game_fn_define_h

global define
define:
	mov rdi, 42
	call game_fn_define_h wrt ..plt
	ret

global init_globals
init_globals:
	mov dword rdi[byte 0x0], 420
	mov dword rdi[0x4], 1337
	ret
