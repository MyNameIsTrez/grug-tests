global define_type
global globals_size

global define
global init_globals

global on_fns

section .data

define_type: db "i", 0

globals_size: dq 0

on_fns:
	dq 0
	dq 0

section .text

extern define_i

define:
	mov rdi, 42
	mov rsi, 69
	call define_i wrt ..plt
	ret

init_globals:
	ret
