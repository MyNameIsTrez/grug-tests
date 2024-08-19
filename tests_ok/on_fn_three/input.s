section .data

global define_type
define_type: db "j", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a
	dq on_b
	dq on_c

section .text

extern game_fn_define_j
extern grug_enable_on_fn_runtime_error_handling
extern grug_disable_on_fn_runtime_error_handling

global define
define:
	call game_fn_define_j wrt ..plt
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

	call grug_disable_on_fn_runtime_error_handling wrt ..plt
	mov rsp, rbp
	pop rbp
	ret

global on_b
on_b:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt
	mov rsp, rbp
	pop rbp
	ret

global on_c
on_c:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt
	mov rsp, rbp
	pop rbp
	ret
