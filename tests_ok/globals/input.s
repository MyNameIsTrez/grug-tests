global define_type

global define
global get_globals_size
global init_globals

global on_fns

section .data

define_type: db "h", 0

on_fns:
	dq 0

section .text

extern define_h

define:
	mov rdi, 42
	call define_h wrt ..plt
	ret

get_globals_size:
	mov eax, 8
	ret

init_globals:
	mov dword [byte rdi+0], 420
	mov dword [rdi+4], 1337
	ret
