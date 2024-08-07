section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

section .text

extern define_d
extern initialize

global define
define:
	call define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
	mov rax, 9223372036854775807
	push rax
	mov rax, 1
	pop r11
	add rax, r11
	push rax

	pop rdi
	call initialize wrt ..plt
	ret
