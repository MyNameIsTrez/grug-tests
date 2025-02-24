section .data

global define_type
define_type: db "m", 0

align 8
global globals_size
globals_size: dq 8

name:
	db "foobar", 0
group:
	db "bar", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern game_fn_define_m

global define
define:
	sub rsp, byte 0x8
	mov eax, 1337
	mov r9, rax

	xor eax, eax
	mov r8, rax

	lea rax, [rel name]
	mov rcx, rax

	mov eax, 1
	mov rdx, rax

	lea rax, [rel group]
	mov rsi, rax

	mov eax, 42
	mov rdi, rax

	call game_fn_define_m wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
