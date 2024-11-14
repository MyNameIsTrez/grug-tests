section .data

global define_type
define_type: db "m", 0

align 8
global globals_size
globals_size: dq 8

global strings
strings:
	db "foobar", 0
	db "bar", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern game_fn_define_m

global define
define:
	mov eax, 1337
	mov r9, rax

	xor eax, eax
	mov r8, rax

	lea rax, strings[rel 0]
	mov rcx, rax

	mov eax, 1
	mov rdx, rax

	lea rax, strings[rel 7]
	mov rsi, rax

	mov eax, 42
	mov rdi, rax

	call game_fn_define_m wrt ..plt
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
