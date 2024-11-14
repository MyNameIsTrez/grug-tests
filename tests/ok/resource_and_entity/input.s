section .data

global define_type
define_type: db "b2", 0

align 8
global globals_size
globals_size: dq 8

global strings
strings:
	db 0
	db "ok:foo", 0
	db "tests/ok/resource_and_entity/foo.txt", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq strings + 8

global entities_size
entities_size: dq 1

global entities
entities:
	dq strings + 1

global entity_types
entity_types:
	dq strings + 0

section .text

extern game_fn_define_b2

global define
define:
	lea rax, strings[rel 1]
	mov rsi, rax
	lea rax, strings[rel 8]
	mov rdi, rax
	call game_fn_define_b2 wrt ..plt
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
