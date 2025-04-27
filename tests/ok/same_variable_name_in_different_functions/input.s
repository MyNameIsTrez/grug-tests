section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a
	dq on_b

on_fn_path:
	db "tests/ok/same_variable_name_in_different_functions/input-e.grug", 0
on_fn_name_a:
	db "on_a", 0
on_fn_name_b:
	db "on_b", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern grug_max_rsp
extern grug_max_time
extern grug_fn_path
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern grug_current_time
extern clock_gettime
extern setjmp
extern game_fn_initialize
extern grug_call_runtime_error_handler
extern longjmp
extern game_fn_sin

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

	save_on_fn_name_and_path_on_a

	set_max_rsp

	set_time_limit

	clear_has_runtime_error_happened

	; foo: i32 = 42
	mov eax, 42
	mov rbp[-0xc], eax

	; initialize(foo)
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	check_game_fn_error

	; bar()
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_bar_safe
	return_if_runtime_error

	mov rsp, rbp
	pop rbp
	ret

.fast:
	; foo: i32 = 42
	mov eax, 42
	mov rbp[-0xc], eax

	; initialize(foo)
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	; bar()
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_bar_fast

	mov rsp, rbp
	pop rbp
	ret

global on_b
on_b:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_on_fn_name_and_path_on_b

	clear_has_runtime_error_happened

	; foo: i32 = 1337
	mov eax, 1337
	mov rbp[-0xc], eax

	; initialize(foo)
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

.fast:
	; foo: i32 = 1337
	mov eax, 1337
	mov rbp[-0xc], eax

	; initialize(foo)
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

global helper_bar_safe
helper_bar_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	; foo: i32 = 69
	mov eax, 69
	mov rbp[-0xc], eax

	; initialize(foo)
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

global helper_bar_fast
helper_bar_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	; foo: i32 = 69
	mov eax, 69
	mov rbp[-0xc], eax

	; initialize(foo)
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
