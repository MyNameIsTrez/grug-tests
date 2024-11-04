section .data

global define_type
define_type: db "v", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "tests/ok/resource_twice/bar.txt", 0
	db "tests/ok/resource_twice/foo.txt", 0

align 8
global resources_size
resources_size: dq 2

global resources
resources:
	dq strings + 0
	dq strings + 32

global entities_size
entities_size: dq 0

section .text

extern game_fn_define_v

global define
define:
	lea rax, strings[rel 0]
	mov rsi, rax
	lea rax, strings[rel 32]
	mov rdi, rax
	call game_fn_define_v wrt ..plt
	ret

global init_globals
init_globals:
	ret
