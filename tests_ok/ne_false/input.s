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
	sub rsp, byte 0x10
    mov rbp[-0x8], rdi

	mov eax, 2
	push rax
	mov eax, 2
	pop r11
	cmp rax, r11
	mov eax, 0
	setne al
	push rax

	pop rdi
	call game_fn_initialize wrt ..plt

    mov rsp, rbp
    pop rbp
	ret
