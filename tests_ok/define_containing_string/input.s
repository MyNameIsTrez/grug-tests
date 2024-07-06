global define_type
global globals_size
global strings

global define
global init_globals

section .data

define_type: db "k", 0

globals_size: dq 0

strings:
	db "foo", 0

section .text

extern define_k

define:
	mov rdi, 42
	lea rsi, [rel strings+0]
	call define_k wrt ..plt
	ret

init_globals:
	ret
