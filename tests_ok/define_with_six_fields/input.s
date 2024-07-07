section .data

global define_type
define_type: db "m", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "military", 0
	db "foo", 0

section .text

extern define_m

global define
define:
	mov rdi, 42
	lea rsi, [rel strings+0]
	mov rdx, 69
	lea rcx, [rel strings+9]
	mov r8, 666
	mov r9, 1337
	call define_m wrt ..plt
	ret

global init_globals
init_globals:
	ret
