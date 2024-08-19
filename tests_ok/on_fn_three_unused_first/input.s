section .data

global define_type
define_type: db "j", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq 0
	dq on_b
	dq on_c

section .text

extern game_fn_define_j
extern alarm

global define
define:
	call game_fn_define_j wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_b
on_b:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	mov edi, 1
	call alarm wrt ..plt

	xor edi, edi
	call alarm wrt ..plt
	mov rsp, rbp
	pop rbp
	ret

global on_c
on_c:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	mov edi, 1
	call alarm wrt ..plt

	xor edi, edi
	call alarm wrt ..plt
	mov rsp, rbp
	pop rbp
	ret
