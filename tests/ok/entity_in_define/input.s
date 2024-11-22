section .data

global define_type
define_type: db "x", 0

align 8
global globals_size
globals_size: dq 8

global strings
strings:
	db 0
	db "ok:foo", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 1

global entities
entities:
	dq strings + 1

global entity_types
entity_types:
	dq strings + 0

section .text

extern game_fn_define_x

global define
define:
	sub rsp, byte 0x8
	lea rax, strings[rel 1]
	mov rdi, rax
	call game_fn_define_x wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
