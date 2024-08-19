section .data

global define_type
define_type: db "r", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

section .text

extern game_fn_define_r
extern alarm
extern game_fn_sin

global define
define:
	call game_fn_define_r wrt ..plt
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
	movss rbp[-0xc], xmm0
	mov edi, 1
	call alarm wrt ..plt

	mov eax, rbp[-0xc]
	push rax

	pop rax
	movd xmm0, eax
	call game_fn_sin wrt ..plt
	movd eax, xmm0

	xor edi, edi
	call alarm wrt ..plt
	mov rsp, rbp
	pop rbp
	ret
