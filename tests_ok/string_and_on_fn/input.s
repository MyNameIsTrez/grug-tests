section .data

global define_type
define_type: db "p", 0

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

extern game_fn_define_p
extern grug_enable_on_fn_runtime_error_handling
extern grug_disable_on_fn_runtime_error_handling

global define
define:
	lea rax, strings[rel 0]
	mov rdi, rax
	call game_fn_define_p wrt ..plt
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
