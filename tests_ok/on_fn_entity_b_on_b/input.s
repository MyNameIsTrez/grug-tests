global define_type

global define
global get_globals_size
global init_globals

global on_fns

global on_b

section .data

define_type: db "i", 0

on_fns:
	dq 0
	dq on_b

section .text

extern define_i

define:
	mov rdi, 42
	mov rsi, 69
	call define_i wrt ..plt
	ret

get_globals_size:
	mov eax, 0
	ret

init_globals:
	ret

on_b:
	ret
