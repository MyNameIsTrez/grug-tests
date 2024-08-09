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

	xor eax, eax
	mov rbp[-0xc], eax

	mov eax, 1
	test rax, rax
	je strict $+0x73
	call game_fn_nothing wrt ..plt
	mov eax, 2
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11
	mov eax, 0
	setne al
	test rax, rax
	je strict $+0x45
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	add rax, r11
	mov rbp[-0xc], eax
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11
	mov eax, 0
	sete al
	test rax, rax
	je strict $+0xb
	jmp strict $-0x4f
	call game_fn_nothing wrt ..plt
	jmp strict $-0x59
	jmp strict $+0xa
	jmp strict $-0x76

	mov rsp, rbp
	pop rbp
	ret
