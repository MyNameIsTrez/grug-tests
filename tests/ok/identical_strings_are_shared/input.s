section .data

global define_type
define_type: db "q", 0

align 8
global globals_size
globals_size: dq 8

b:
	db "b", 0
a:
	db "a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern game_fn_define_q

global define
define:
	sub rsp, byte 0x8
	lea rax, [rel b]
	mov rdx, rax
	lea rax, [rel b]
	mov rsi, rax
	lea rax, [rel a]
	mov rdi, rax
	call game_fn_define_q wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
