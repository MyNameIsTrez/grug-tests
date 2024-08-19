section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db "temp", 0
	db "foo", 0

section .text

extern game_fn_define_d
extern grug_enable_on_fn_runtime_error_handling
extern game_fn_say
extern grug_disable_on_fn_runtime_error_handling

global define
define:
	call game_fn_define_d wrt ..plt
	ret

global init_globals
init_globals:
	lea rax, strings[rel 0]
	mov rdi[byte 0x0], rax
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	lea rax, strings[rel 5]
	mov r11, rbp[-0x8]
	mov r11[byte 0], rax

	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x0]
	push rax

	pop rdi
	call game_fn_say wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt
	mov rsp, rbp
	pop rbp
	ret
