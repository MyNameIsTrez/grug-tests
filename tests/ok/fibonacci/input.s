section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/fibonacci/input.grug", 0
on_fn_name:
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern grug_runtime_error_handler
extern grug_max_rsp
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern game_fn_define_d
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_initialize
extern longjmp

%define GRUG_ON_FN_STACK_OVERFLOW 1
%define GRUG_ON_FN_OVERFLOW 3
%define GRUG_ON_FN_UNDERFLOW 4

%define GRUG_STACK_LIMIT 0x10000

global define
define:
	sub rsp, byte 0x8
	call game_fn_define_d wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret

%macro save_on_fn_name_and_path 0
	mov rax, [rel grug_on_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, [rel on_fn_name]
	mov [rax], r11
%endmacro

%macro set_max_rsp 0
	mov rax, [rel grug_max_rsp wrt ..got]
	mov [rax], rsp
	sub qword [rax], GRUG_STACK_LIMIT
%endmacro

%macro error_handling 0
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call setjmp wrt ..plt
	test eax, eax
	je %%skip

	dec eax
	push rax
	mov edi, eax
	sub rsp, byte 0x8
	call grug_get_runtime_error_reason wrt ..plt
	add rsp, byte 0x8
	mov rdi, rax

	lea rcx, [rel on_fn_path]

	lea rdx, [rel on_fn_name]

	pop rsi

	mov rax, [rel grug_runtime_error_handler wrt ..got]
	call [rax]

	mov rsp, rbp
	pop rbp
	ret
%%skip:
%endmacro

%macro check_stack_overflow 0
	mov rax, [rel grug_max_rsp wrt ..got]
	cmp rsp, [rax]
	jg %%skip
	mov esi, 1 + GRUG_ON_FN_STACK_OVERFLOW
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call longjmp wrt ..plt
%%skip:
%endmacro

%macro check_overflow_and_underflow 0
	jno %%skip
	js %%signed ; 2147483647 + 1 is signed, since it overflows to -2147483648
	mov esi, 1 + GRUG_ON_FN_UNDERFLOW
	jmp short %%skip_signed
%%signed:
	mov esi, 1 + GRUG_ON_FN_OVERFLOW
%%skip_signed:
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call longjmp wrt ..plt
%%skip:
%endmacro

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

	error_handling

	mov eax, 10
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	call helper_fib_safe
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 10
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

	; if n == 0
	xor eax, eax
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11
	mov eax, 0
	sete al
	test eax, eax
	je .or_false

	; or n == 1
	mov eax, 1
	jmp strict .early_return
.or_false:
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11
	mov eax, 0
	sete al
	test eax, eax
	mov eax, 0
	setne al
.early_return:
	test eax, eax
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
	sub rax, r11
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	call helper_fib_safe
	push rax

	; helper_fib_safe(n - 1)
	sub rsp, byte 0x8
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	sub rax, r11
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	call helper_fib_safe
	add rsp, byte 0x8
	pop r11

	; helper_fib_safe(n - 1) + helper_fib_safe(n - 2)
	add eax, r11d
	check_overflow_and_underflow

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
	cmp rax, r11
	mov eax, 0
	sete al
	test eax, eax
	je .or_false_fast

	; or n == 1
	mov eax, 1
	jmp strict .early_return_fast
.or_false_fast:
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11
	mov eax, 0
	sete al
	test eax, eax
	mov eax, 0
	setne al
.early_return_fast:
	test eax, eax
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
	sub rax, r11
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
	sub rax, r11
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
