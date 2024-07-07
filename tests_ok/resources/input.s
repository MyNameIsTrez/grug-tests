global define_type
global globals_size

global define
global init_globals

global on_fns

section .data

define_type: db "h", 0

align 8
globals_size: dq 0

on_fns:
	dq 0

section .text

extern define_h

define:
	mov rdi, 42
	call define_h wrt ..plt
	ret

init_globals:
	ret
