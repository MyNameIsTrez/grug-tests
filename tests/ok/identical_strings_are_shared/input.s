section .data

global define_type
define_type: db "q", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "b", 0
	db "a", 0

section .text

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_block_mask

extern game_fn_define_q

global define
define:
	lea rax, strings[rel 0]
	mov rdx, rax
	lea rax, strings[rel 0]
	mov rsi, rax
	lea rax, strings[rel 2]
	mov rdi, rax
	call game_fn_define_q wrt ..plt
	ret

global init_globals
init_globals:
	ret
