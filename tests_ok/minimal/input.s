global define
global define_type

global get_globals_struct_size

section .data

define: dq 42

define_type: db "entity", 0

section .text

get_globals_struct_size:
	mov rax, 8
	ret