global define_type

global define
global get_globals_size
global init_globals

section .data

define_type: db "entity", 0

section .text

extern define_entity

define:
	mov rdi, 42
	call define_entity wrt ..plt
	ret

get_globals_size:
	mov eax, 8
	ret

init_globals:
	mov dword [rdi+0], 420
	mov dword [rdi+4], 1337
	ret
