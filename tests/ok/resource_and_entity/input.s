section .data

global define_type
define_type: db "b2", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "ok:foo", 0
	db "tests/ok/resource_and_entity/foo.txt", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq strings + 7

global entities_size
entities_size: dq 1

global entities
entities:
	dq strings + 0

section .text

extern game_fn_define_b2

global define
define:
	lea rax, strings[rel 0]
	mov rsi, rax
	lea rax, strings[rel 7]
	mov rdi, rax
	call game_fn_define_b2 wrt ..plt
	ret

global init_globals
init_globals:
	ret
