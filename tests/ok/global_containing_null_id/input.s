section .data

align 8
global globals_size
globals_size: dq 16

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/global_containing_null_id/input-d.grug", 0
on_fn_name:
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern grug_runtime_error_handler
extern grug_fn_path
extern grug_runtime_error_jmp_buffer
extern grug_fn_name
extern grug_on_fns_in_safe_mode
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_set_target

global init_globals
init_globals:
	mov rdi[0x0], rsi
	mov rax, -1
	mov rdi[0x8], rax
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_on_fn_name_and_path

	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
