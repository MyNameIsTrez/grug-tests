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
extern nothing

global define
define:
	call define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
    push rbp
    mov rbp, rsp
	sub rsp, 0x8
    mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	call foo

	call nothing wrt ..plt

    mov rsp, rbp
    pop rbp
	ret

global foo
foo:
    push rbp
    mov rbp, rsp
	sub rsp, 0x8
    mov rbp[-0x8], rdi

    mov rsp, rbp
    pop rbp
	ret

    mov rsp, rbp
    pop rbp
	ret
