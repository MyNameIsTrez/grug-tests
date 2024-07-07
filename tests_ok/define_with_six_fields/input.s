global define_type
global globals_size
global strings

global define
global init_globals

section .data

define_type: db "m", 0

align 8
globals_size: dq 0

strings:
	db "military", 0
	db "foo", 0

section .text

extern define_m

define:
	mov rdi, 42
	lea rsi, [rel strings+0]
	mov rdx, 69
	lea rcx, [rel strings+9]
	mov r8, 666
	mov r9, 1337
	call define_m wrt ..plt
	ret

init_globals:
	ret
