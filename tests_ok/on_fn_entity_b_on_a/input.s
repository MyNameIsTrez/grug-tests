global define_type
global globals_size

global define
global init_globals

global on_fns

global on_a

section .data

define_type: db "i", 0

align 8
globals_size: dq 0

on_fns:
	dq on_a
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

on_a:
	ret
