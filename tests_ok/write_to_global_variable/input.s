section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

section .text

extern game_fn_define_d
extern grug_enable_on_fn_runtime_error_handling
extern game_fn_max
extern grug_disable_on_fn_runtime_error_handling

global define
define:
	call game_fn_define_d wrt ..plt
	ret

global init_globals
init_globals:
	mov eax, 42
	mov rdi[byte 0x0], eax
	mov eax, 69
	mov rdi[0x4], eax
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	; foo + 1
	mov eax, 1
	push rax
	mov rax, rbp[-0x8]
	mov eax, rax[byte 0x0]
	pop r11
	add rax, r11

	; foo = foo + 1
	mov r11, rbp[-0x8]
	mov r11[byte 0x0], eax

	; push foo
	mov rax, rbp[-0x8]
	mov eax, rax[byte 0x0]
	push rax

	; push bar
	mov rax, rbp[-0x8]
	mov eax, rax[0x4]
	push rax

	pop rsi
	pop rdi
	call game_fn_max wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt
	mov rsp, rbp
	pop rbp
	ret
