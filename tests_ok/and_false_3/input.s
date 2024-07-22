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
	xor eax, eax
	test rax, rax
	jne strict $+0xb
	jmp strict $+0x12
	xor eax, eax
	test rax, rax
	mov eax, 0
	setne al

	push rax

	pop rdi
	call initialize wrt ..plt
	ret
