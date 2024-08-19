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
extern game_fn_magic
extern game_fn_initialize
extern game_fn_identity
extern game_fn_max
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

	call game_fn_nothing wrt ..plt
	call game_fn_magic wrt ..plt

	mov eax, 42
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov eax, 69
	push rax
	pop rdi
	call game_fn_identity wrt ..plt

	mov eax, 1337
	push rax
	mov eax, 8192
	push rax
	pop rsi
	pop rdi
	call game_fn_max wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt
	mov rsp, rbp
	pop rbp
	ret
