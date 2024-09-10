section .data

global define_type
define_type: db "o", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "z", 0
	db "y", 0
	db "x", 0
	db "w", 0
	db "v", 0
	db "u", 0

align 8
global dll_resources_size
dll_resources_size: dq 0

section .text

extern game_fn_define_o

global define
define:
	lea rax, strings[rel 0]
	mov r9, rax
	lea rax, strings[rel 2]
	mov r8, rax
	lea rax, strings[rel 4]
	mov rcx, rax
	lea rax, strings[rel 6]
	mov rdx, rax
	lea rax, strings[rel 8]
	mov rsi, rax
	lea rax, strings[rel 10]
	mov rdi, rax
	call game_fn_define_o wrt ..plt
	ret

global init_globals
init_globals:
	ret
