global define_type

global define
global get_globals_size
global init_globals

section .data

define_type: db "a", 0

section .text

extern define_a

define:
	mov rdi, 42
	call define_a wrt ..plt
	ret

get_globals_size:
	mov eax, 0
	ret

init_globals:
	ret
