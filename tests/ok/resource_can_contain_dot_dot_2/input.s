section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/resource_can_contain_dot_dot_2/input-d.grug", 0
on_fn_name:
	db "on_a", 0
resource:
	db "tests/ok/resource_can_contain_dot_dot_2/foo..", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq resource

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern grug_fn_path
extern grug_fn_name
extern grug_has_game_function_error_happened
extern grug_on_fns_in_safe_mode
extern game_fn_draw

global init_globals
init_globals:
	mov rdi[0x0], rsi
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

	lea rax, [rel resource]
	push rax
	pop rdi
	call game_fn_draw wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	lea rax, [rel resource]
	push rax
	pop rdi
	call game_fn_draw wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
