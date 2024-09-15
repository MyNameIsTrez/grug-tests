section .data

global define_type
define_type: db "k", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "foo", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern game_fn_define_k

global define
define:
	lea rax, strings[rel 0]
	mov rsi, rax
	mov eax, 42
	mov rdi, rax
	call game_fn_define_k wrt ..plt
	ret

global init_globals
init_globals:
	ret
