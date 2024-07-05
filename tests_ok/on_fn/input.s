global define_type
global globals_size

global define
global init_globals

global on_fns

global on_a

section .data

define_type: db "h", 0

globals_size: dq 0

on_fns:
	dq on_a

section .text

extern define_h

define:
	mov rdi, 42
	call define_h wrt ..plt
	ret

init_globals:
	ret

on_a:
	ret
