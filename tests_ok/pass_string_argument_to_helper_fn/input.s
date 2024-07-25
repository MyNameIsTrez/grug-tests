section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db "foo", 0

section .text

extern define_d
extern say

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

	lea rax, [rel strings+0]
	push rax

	pop rsi
	pop rdi
    call shout

    mov rsp, rbp
    pop rbp
	ret

global shout
shout:
    push rbp
    mov rbp, rsp
    sub rsp, 0x10
    mov rbp[-0x8], rdi

    mov rbp[-0x10], rsi
    mov rax, rbp[-0x10]
    push rax

	pop rdi
	call say wrt ..plt

    mov rsp, rbp
    pop rbp
    ret
