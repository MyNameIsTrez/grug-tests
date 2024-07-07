global define_type
global globals_size

global define
global init_globals

global on_fns

section .data

define_type: db "h", 0

align 8

globals_size: dq 8

on_fns:
	dq 0

section .text

extern define_h

define:
	mov rdi, 42
	call define_h wrt ..plt
	ret

init_globals:
	mov dword [byte rdi+0], 420
	mov dword [rdi+4], 1337
	ret
