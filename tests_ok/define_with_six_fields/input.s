section .data

global define_type
define_type: db "m", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "bar", 0
	db "foobar", 0

section .text

extern grug_block_mask

extern game_fn_define_m

global define
define:
	mov eax, 1337
	mov r9, rax

	xor eax, eax
	mov r8, rax

	lea rax, strings[rel 4]
	mov rcx, rax

	mov eax, 1
	mov rdx, rax

	lea rax, strings[rel 0]
	mov rsi, rax

	mov eax, 42
	mov rdi, rax

	call game_fn_define_m wrt ..plt
	ret

global init_globals
init_globals:
	ret
