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
	mov edi, 42
	lea rsi, strings[rel 0]
	mov edx, 1
	lea rcx, strings[rel 4]
	xor r8d, r8d
	mov r9d, 1337
	call game_fn_define_m wrt ..plt
	ret

global init_globals
init_globals:
	ret
