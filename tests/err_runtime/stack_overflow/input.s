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
	db "tests/err_runtime/stack_overflow/input.grug", 0
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
extern longjmp

%define GRUG_ON_FN_STACK_OVERFLOW 1

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
	lea r11, strings[rel 0]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, strings[rel 44]
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

	lea rcx, strings[rel 0]

	lea rdx, strings[rel 44]

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

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je .fast

	save_on_fn_name_and_path

	set_max_rsp

	error_handling

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	call helper_foo

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov rax, rbp[-0x8]
	push rax

	pop rdi
	call helper_foo

	mov rsp, rbp
	pop rbp
	ret

global helper_foo
helper_foo:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	call helper_foo

	mov rsp, rbp
	pop rbp
	ret
