section .data

global define_type
define_type: db "b", 0

align 8
global globals_size
globals_size: dq 8

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern game_fn_define_b

global define
define:
	sub rsp, byte 0x8
	mov eax, 2
	neg eax
	mov rdi, rax
	call game_fn_define_b wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
