global define_type
global define

global get_globals_size
global init_globals

section .data

define_type: db "entity", 0

define:
	dq 42
	dq 69

section .text

get_globals_size:
	mov eax, 0
	ret

init_globals:
	ret
