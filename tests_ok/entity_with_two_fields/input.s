global define_type

global define
global get_globals_size
global init_globals

section .data

define_type: db "b", 0

section .text

extern define_b

define:
	mov rdi, 42
	mov rsi, 69
	call define_b wrt ..plt
	ret

get_globals_size:
	mov eax, 0
	ret

init_globals:
	ret
