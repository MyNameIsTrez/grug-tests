section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/err_runtime/time_limit_exceeded_fibonacci/input-d.grug", 0
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
extern grug_max_rsp
extern grug_max_time
extern grug_fn_path
extern grug_runtime_error_jmp_buffer
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern grug_current_time
extern clock_gettime
extern setjmp
extern game_fn_initialize
extern grug_get_runtime_error_reason
extern longjmp

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret

global on_a:
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

	set_time_limit

	clear_has_runtime_error_happened

	mov eax, 100
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	call helper_fib_safe
	return_if_runtime_error
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 100
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	call helper_fib_fast
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

global helper_fib_safe:
helper_fib_safe:
	; Function prologue
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	mov rbp[-0xc], esi
	check_stack_overflow
	check_time_limit_exceeded

	; if n == 0
	xor eax, eax
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp eax, r11d
	mov eax, 0
	sete al
	test al, al
	je .or_false

	; or n == 1
	mov eax, 1
	jmp strict .early_return
.or_false:
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp eax, r11d
	mov eax, 0
	sete al
	test al, al
	mov eax, 0
	setne al
.early_return:
	test al, al
	je strict .dont_early_return

	; return n
	mov eax, rbp[-0xc]
	mov rsp, rbp
	pop rbp
	ret

.dont_early_return:
	; helper_fib_safe(n - 2)
	mov eax, 2
	push rax
	mov eax, rbp[-0xc]
	pop r11
	sub eax, r11d
	check_overflow
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	call helper_fib_safe
	return_if_runtime_error
	push rax

	; helper_fib_safe(n - 1)
	sub rsp, byte 0x8
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	sub eax, r11d
	check_overflow
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	call helper_fib_safe
	add rsp, byte 0x8
	return_if_runtime_error
	pop r11

	; helper_fib_safe(n - 1) + helper_fib_safe(n - 2)
	add eax, r11d
	check_overflow

	; return helper_fib_safe(n - 1) + helper_fib_safe(n - 2)
	mov rsp, rbp
	pop rbp
	ret

	; Function epilogue
	mov rsp, rbp
	pop rbp
	ret

global helper_fib_fast:
helper_fib_fast:
	; Function prologue
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	mov rbp[-0xc], esi

	; if n == 0
	xor eax, eax
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp eax, r11d
	mov eax, 0
	sete al
	test al, al
	je .or_false_fast

	; or n == 1
	mov eax, 1
	jmp strict .early_return_fast
.or_false_fast:
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp eax, r11d
	mov eax, 0
	sete al
	test al, al
	mov eax, 0
	setne al
.early_return_fast:
	test al, al
	je strict .dont_early_return

	; return n
	mov eax, rbp[-0xc]
	mov rsp, rbp
	pop rbp
	ret

.dont_early_return:
	; helper_fib_fast(n - 2)
	mov eax, 2
	push rax
	mov eax, rbp[-0xc]
	pop r11
	sub eax, r11d
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	call helper_fib_fast
	push rax

	; helper_fib_fast(n - 1)
	sub rsp, byte 0x8
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	sub eax, r11d
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	call helper_fib_fast
	add rsp, byte 0x8
	pop r11

	; helper_fib_fast(n - 1) + helper_fib_fast(n - 2)
	add eax, r11d

	; return helper_fib_fast(n - 1) + helper_fib_fast(n - 2)
	mov rsp, rbp
	pop rbp
	ret

	; Function epilogue
	mov rsp, rbp
	pop rbp
	ret
