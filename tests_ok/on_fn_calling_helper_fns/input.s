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
extern game_fn_nothing
extern game_fn_initialize

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
	sub rsp, 0x10
    mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	call helper_foo

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	call helper_bar

    mov rsp, rbp
    pop rbp
	ret

global helper_foo
helper_foo:
    push rbp
    mov rbp, rsp
	sub rsp, 0x10
    mov rbp[-0x8], rdi

	call game_fn_nothing wrt ..plt

    mov rsp, rbp
    pop rbp
	ret

global helper_bar
helper_bar:
    push rbp
    mov rbp, rsp
	sub rsp, 0x10
    mov rbp[-0x8], rdi

	mov eax, 42
	push rax

	pop rdi
	call game_fn_initialize wrt ..plt

    mov rsp, rbp
    pop rbp
	ret
