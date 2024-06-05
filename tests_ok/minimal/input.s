global define
global define_type

global get_globals_struct_size
global init_globals_struct

section .data

define: dq 42

define_type: db "entity", 0

section .text

get_globals_struct_size:
	mov eax, 0
	ret

init_globals_struct:
	ret
