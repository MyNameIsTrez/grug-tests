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

extern game_fn_define_d
extern game_fn_initialize_bool

global define
define:
	call game_fn_define_d wrt ..plt
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

	; true
	mov eax, 1

	; !eax
	test rax, rax
	mov eax, 0
    sete al
	push rax

	pop rdi
	call game_fn_initialize_bool wrt ..plt

    mov rsp, rbp
    pop rbp
	ret