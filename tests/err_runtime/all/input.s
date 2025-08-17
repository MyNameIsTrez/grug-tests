section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/err_runtime/all/input-D.grug", 0
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
extern grug_call_runtime_error_handler
extern game_fn_initialize
extern grug_is_time_limit_exceeded
extern clock_gettime
extern grug_get_max_rsp

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

	xor eax, eax
	push rax
	mov eax, 1
	pop r11
	check_division_by_0
	check_division_overflow
	cdq
	idiv r11d
	push rax

	pop rdi
	call game_fn_initialize wrt ..plt
	check_game_fn_error

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	call helper_foo_safe
	return_if_runtime_error

.repeat:
	mov eax, 1
	test al, al
	je strict .skip

	check_time_limit_exceeded
	jmp strict .repeat
.skip:

	mov eax, 1
	push rax
	mov eax, 1
	pop r11
	add eax, r11d
	check_overflow
	mov rbp[-0xc], eax

	mov rsp, rbp
	pop rbp
	ret

.fast:
	xor eax, eax
	push rax
	mov eax, 1
	pop r11
	cdq
	idiv r11d
	push rax

	pop rdi
	call game_fn_initialize wrt ..plt

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	call helper_foo_fast

.repeat_fast:
	mov eax, 1
	test al, al
	je strict .skip_fast

	jmp strict .repeat_fast
.skip_fast:

	mov eax, 1
	push rax
	mov eax, 1
	pop r11
	add eax, r11d
	mov rbp[-0xc], eax

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_safe
helper_foo_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	call helper_foo_safe
	return_if_runtime_error

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_fast
helper_foo_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	call helper_foo_fast

	mov rsp, rbp
	pop rbp
	ret
