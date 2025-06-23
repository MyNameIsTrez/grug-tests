section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/f32_passed_to_helper_fn/input-d.grug", 0
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

extern grug_max_time
extern grug_fn_path
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern grug_current_time
extern grug_get_max_rsp_addr
extern grug_set_time_limit
extern grug_get_max_rsp
extern setjmp
extern grug_call_runtime_error_handler
extern grug_is_time_limit_exceeded
extern clock_gettime
extern game_fn_sin
extern longjmp

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

	set_max_rsp

	call grug_set_time_limit wrt ..plt

	clear_has_runtime_error_happened

	mov eax, __?float32?__(42.0)
	push rax

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	pop rax
	movd xmm0, eax
	call helper_foo_safe
	return_if_runtime_error

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, __?float32?__(42.0)
	push rax

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	pop rax
	movd xmm0, eax
	call helper_foo_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_safe
helper_foo_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	movss rbp[-0xc], xmm0
	check_stack_overflow
	check_time_limit_exceeded

	mov eax, rbp[-0xc]
	push rax

	pop rax
	movd xmm0, eax
	call game_fn_sin wrt ..plt
	movd eax, xmm0
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_fast
helper_foo_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	movss rbp[-0xc], xmm0

	mov eax, rbp[-0xc]
	push rax

	pop rax
	movd xmm0, eax
	call game_fn_sin wrt ..plt
	movd eax, xmm0

	mov rsp, rbp
	pop rbp
	ret
