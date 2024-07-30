section .data

global define_type
define_type: db "m", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "bar", 0
	db "foobar", 0

section .text

extern game_fn_define_m

global define
define:
	mov rdi, 42
	lea rsi, strings[rel 0]
	mov rdx, 69
	lea rcx, strings[rel 4]
	mov r8, 666
	mov r9, 1337
	call game_fn_define_m wrt ..plt
	ret

global init_globals
init_globals:
	ret
