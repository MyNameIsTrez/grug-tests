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
extern max

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

	mov eax, 2
	push rax
	mov eax, 1
	pop r11
	add rax, r11
	push rax

	mov eax, 5
	push rax
	mov eax, 4
	pop r11
	add rax, r11
	push rax

	pop rsi
	pop rdi
	call max wrt ..plt

    mov rsp, rbp
    pop rbp
	ret
