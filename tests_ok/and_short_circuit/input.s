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
extern alarm
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
	mov edi, 1
	call alarm wrt ..plt

	xor eax, eax
	test eax, eax
	jne $+0x7
	jmp strict $+0x2e
	xor eax, eax
	push rax
	xor eax, eax
	push rax
	mov eax, 1
	pop r11
	cqo
	idiv r11
	pop r11
	cmp rax, r11
	mov eax, 0
	sete al
	test eax, eax
	mov eax, 0
	setne al

	push rax

	pop rdi
	call game_fn_initialize_bool wrt ..plt

	xor edi, edi
	call alarm wrt ..plt
    mov rsp, rbp
    pop rbp
	ret
