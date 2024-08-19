section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db "foo", 0

section .text

extern game_fn_define_d
extern grug_enable_on_fn_runtime_error_handling
extern strcmp
extern game_fn_initialize_bool
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

	lea rax, strings[rel 0]
	push rax

	lea rax, strings[rel 0]
	pop r11

	mov rsi, r11
	mov rdi, rax
	call strcmp wrt ..plt

	test eax, eax
	setne al
	push rax

	pop rdi
	call game_fn_initialize_bool wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt
	mov rsp, rbp
	pop rbp
	ret
