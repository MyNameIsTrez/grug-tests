section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/nested_break/input-d.grug", 0
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
extern clock_gettime
extern setjmp
extern game_fn_nothing
extern grug_call_runtime_error_handler
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

	set_time_limit

	clear_has_runtime_error_happened

	call game_fn_nothing wrt ..plt
	check_game_fn_error
.repeat_outer:
	mov eax, 1
	test al, al
	je strict .skip_outer
.repeat_inner:
	mov eax, 1
	test al, al
	je strict .skip_inner
	jmp strict .skip_inner
	check_time_limit_exceeded
	jmp strict .repeat_inner
.skip_inner:
	call game_fn_nothing wrt ..plt
	check_game_fn_error
	jmp strict .skip_outer
	check_time_limit_exceeded
	jmp strict .repeat_outer
.skip_outer:
	call game_fn_nothing wrt ..plt
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

.fast:
	call game_fn_nothing wrt ..plt
.repeat_outer_fast:
	mov eax, 1
	test al, al
	je strict .skip_outer_fast
.repeat_inner_fast:
	mov eax, 1
	test al, al
	je strict .skip_inner_fast
	jmp strict .skip_inner_fast
	jmp strict .repeat_inner_fast
.skip_inner_fast:
	call game_fn_nothing wrt ..plt
	jmp strict .skip_outer_fast
	jmp strict .repeat_outer_fast
.skip_outer_fast:
	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
