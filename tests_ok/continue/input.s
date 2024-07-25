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
	sub rsp, 0xc
    mov rbp[-0x8], rdi

	xor eax, eax
	mov rbp[-0xc], eax

	mov eax, 0x2
	push rax

	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11

	mov eax, 0x0
	setl al

	test rax, rax
	je strict $+0x2b

	call nothing wrt ..plt

	; i++
	mov eax, 0x1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	add rax, r11
	mov rbp[-0xc], eax

	jmp strict $-0x35 ; continue

	call nothing wrt ..plt

	jmp strict $-0x3f ; jump to start of loop

	mov rsp, rbp
	pop rbp
	ret
