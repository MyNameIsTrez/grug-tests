section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/err_runtime/time_limit_exceeded/input-d.grug", 0
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

extern grug_fn_path
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern grug_set_time_limit
extern grug_is_time_limit_exceeded
extern clock_gettime
extern grug_call_runtime_error_handler
extern setjmp
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

	call grug_set_time_limit wrt ..plt

	clear_has_runtime_error_happened

.repeat:
	mov eax, 1
	test al, al
	je strict .skip

	check_time_limit_exceeded
	jmp strict .repeat
.skip:

	mov rsp, rbp
	pop rbp
	ret

.fast:
.repeat_fast:
	mov eax, 1
	test al, al
	je strict .skip_fast

	jmp strict .repeat_fast
.skip_fast:

	mov rsp, rbp
	pop rbp
	ret
