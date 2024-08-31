section .data

global define_type
define_type: db "b", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:

section .text

extern game_fn_define_b

global define
define:
	mov eax, 2
	push rax
	mov eax, 1
	pop r11
	add rax, r11
	mov rdi, rax
	call game_fn_define_b wrt ..plt
	ret

global init_globals
init_globals:
	ret
