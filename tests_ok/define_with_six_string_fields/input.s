section .data

global define_type
define_type: db "o", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "u", 0
	db "v", 0
	db "w", 0
	db "x", 0
	db "y", 0
	db "z", 0

section .text

extern define_o

global define
define:
	lea rdi, strings[rel 0]
	lea rsi, strings[rel 2]
	lea rdx, strings[rel 4]
	lea rcx, strings[rel 6]
	lea r8, strings[rel 8]
	lea r9, strings[rel 10]
	call define_o wrt ..plt
	ret

global init_globals
init_globals:
	ret
