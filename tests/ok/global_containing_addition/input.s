section .data

global define_type
define_type: db "a", 0

align 8
global globals_size
globals_size: dq 12

global strings
strings:

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern game_fn_define_a

global define
define:
	call game_fn_define_a wrt ..plt
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	mov eax, 3
	push rax
	mov eax, 2
	pop r11
	add rax, r11
	mov rdi[0x8], eax
	ret
