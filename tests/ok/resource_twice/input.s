section .data

global define_type
define_type: db "v", 0

align 8
global globals_size
globals_size: dq 8

on_fn_path:
	db "tests/ok/resource_twice/bar.txt", 0
on_fn_name:
	db "tests/ok/resource_twice/foo.txt", 0

align 8
global resources_size
resources_size: dq 2

global resources
resources:
	dq strings + 0
	dq strings + 32

global entities_size
entities_size: dq 0

section .text

extern game_fn_define_v

global define
define:
	sub rsp, byte 0x8
	lea rax, strings[rel 0]
	mov rsi, rax
	lea rax, strings[rel 32]
	mov rdi, rax
	call game_fn_define_v wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
