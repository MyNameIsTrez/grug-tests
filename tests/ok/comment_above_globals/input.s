section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 20

global on_fns
on_fns:
	dq 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern game_fn_define_d

global define
define:
	sub rsp, byte 0x8
	call game_fn_define_d wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi

	mov eax, 420
	mov rdi[0x8], eax

	mov eax, 1337
	mov rdi[0xc], eax

	mov eax, 100
	mov rdi[0x10], eax

	ret
