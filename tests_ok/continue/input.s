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
extern grug_enable_on_fn_runtime_error_handling
extern game_fn_nothing
extern grug_disable_on_fn_runtime_error_handling

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
	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	xor eax, eax
	mov rbp[-0xc], eax

	mov eax, 2
	push rax

	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11

	mov eax, 0x0
	setl al

	test eax, eax
	je strict $+0x2b

	call game_fn_nothing wrt ..plt

	; i++
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	add rax, r11
	mov rbp[-0xc], eax

	jmp strict $-0x34 ; continue

	call game_fn_nothing wrt ..plt

	jmp strict $-0x3e ; jump to start of loop

	call grug_disable_on_fn_runtime_error_handling wrt ..plt
	mov rsp, rbp
	pop rbp
	ret
