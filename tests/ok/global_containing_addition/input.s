section .data

global define_type
define_type: db "a", 0

align 8
global globals_size
globals_size: dq 4

global strings
strings:

section .text

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_block_mask

extern game_fn_define_a

global define
define:
	call game_fn_define_a wrt ..plt
	ret

global init_globals
init_globals:
	mov eax, 3
	push rax
	mov eax, 2
	pop r11
	add rax, r11
	mov rdi[byte 0x0], eax
	ret
