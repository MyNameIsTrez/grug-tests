section .data

global define_type
define_type: db "n", 0

align 8
global globals_size
globals_size: dq 8

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern game_fn_define_n

global define
define:
	sub rsp, byte 0x8
	mov eax, 6
	mov r9, rax
	mov eax, 5
	mov r8, rax
	mov eax, 4
	mov rcx, rax
	mov eax, 3
	mov rdx, rax
	mov eax, 2
	mov rsi, rax
	mov eax, 1
	mov rdi, rax
	call game_fn_define_n wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
