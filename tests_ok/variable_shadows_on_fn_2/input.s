section .data

global define_type
define_type: db "j", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a
	dq 0
	dq 0

section .text

extern game_fn_define_j
extern game_fn_initialize

global define
define:
	call game_fn_define_j wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
    mov rbp[-0x8], rdi

	mov eax, 42
	mov rbp[-0xc], eax

	mov eax, rbp[-0xc]
	push rax

	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
