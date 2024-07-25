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
    push rbp
    mov rbp, rsp
	sub rsp, 0x8
    mov rbp[-0x8], rdi

	mov eax, 1
	test rax, rax
	je $+0xc
	mov eax, 1
	jmp strict $+0x2d
	xor eax, eax
	push rax
	xor eax, eax
	push rax
	mov eax, 1
	pop rbx
	cqo
	idiv rbx
	pop rbx
	cmp rax, rbx
	mov eax, 0
	sete al
	test rax, rax
	mov eax, 0
	setne al

	push rax

	pop rdi
	call initialize wrt ..plt

    mov rsp, rbp
    pop rbp
	ret
