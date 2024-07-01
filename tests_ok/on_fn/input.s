global define_type

global define
global get_globals_size
global init_globals

global on_fns

global on_a

section .data

define_type: db "h", 0

on_fns:
	dq on_a

section .text

extern define_h

define:
	mov rdi, 42
	call define_h wrt ..plt
	ret

get_globals_size:
	mov eax, 0
	ret

init_globals:
	ret

on_a:
	ret
