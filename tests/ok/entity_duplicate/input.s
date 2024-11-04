section .data

global define_type
define_type: db "y", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db 0
	db "ok:foo", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 2

global entities
entities:
	dq strings + 1
	dq strings + 1

global entity_types
entity_types:
	dq strings + 0
	dq strings + 0

section .text

extern game_fn_define_y

global define
define:
	lea rax, strings[rel 1]
	mov rsi, rax
	lea rax, strings[rel 1]
	mov rdi, rax
	call game_fn_define_y wrt ..plt
	ret

global init_globals
init_globals:
	ret
