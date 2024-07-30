section .data

global define_type
define_type: db "j", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a
	dq on_b
	dq 0

section .text

extern game_fn_define_j

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
	sub rsp, 0x10
    mov rbp[-0x8], rdi

    mov rsp, rbp
    pop rbp
	ret

global on_b
on_b:
    push rbp
    mov rbp, rsp
	sub rsp, 0x10
    mov rbp[-0x8], rdi

    mov rsp, rbp
    pop rbp
	ret
