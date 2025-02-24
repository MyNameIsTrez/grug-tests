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
	db "tests/ok/f32_passed_to_helper_fn/input.grug", 0
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
extern grug_max_time
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern grug_current_time
extern game_fn_define_d
extern clock_gettime
extern setjmp
extern grug_get_runtime_error_reason
extern longjmp
extern game_fn_sin

%define GRUG_ON_FN_STACK_OVERFLOW 1
%define GRUG_ON_FN_TIME_LIMIT_EXCEEDED 2

%define GRUG_STACK_LIMIT 0x10000
%define CLOCK_PROCESS_CPUTIME_ID 2
%define TV_SEC_OFFSET 0
%define TV_NSEC_OFFSET 8
%define GRUG_ON_FN_TIME_LIMIT_MS 10
%define NS_PER_MS 1000000
%define NS_PER_SEC 1000000000

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

%macro set_time_limit 0
	mov rsi, [rel grug_max_time wrt ..got]
	push rsi
	mov edi, CLOCK_PROCESS_CPUTIME_ID
	call clock_gettime wrt ..plt
	pop rax
	add qword [byte rax + TV_NSEC_OFFSET], GRUG_ON_FN_TIME_LIMIT_MS * NS_PER_MS
	cmp qword [byte rax + TV_NSEC_OFFSET], NS_PER_SEC
	jl %%skip
	sub qword [byte rax + TV_NSEC_OFFSET], NS_PER_SEC
	inc qword [byte rax + TV_SEC_OFFSET]
%%skip:
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

%macro check_time_limit_exceeded 0
	mov rsi, [rel grug_current_time wrt ..got]
	push rsi
	mov edi, CLOCK_PROCESS_CPUTIME_ID
	call clock_gettime wrt ..plt
	pop rax
	mov r11, [rel grug_max_time wrt ..got]

	mov r10, [byte r11 + TV_SEC_OFFSET]
	cmp [byte rax + TV_SEC_OFFSET], r10
	jl %%skip
	jg %%longjump
	mov r10, [byte r11 + TV_NSEC_OFFSET]
	cmp [byte rax + TV_NSEC_OFFSET], r10
	jg %%longjump
	jmp short %%skip
%%longjump:
	mov esi, 1 + GRUG_ON_FN_TIME_LIMIT_EXCEEDED
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
	je strict .fast

	save_on_fn_name_and_path

	set_max_rsp

	set_time_limit

	error_handling

	mov eax, __?float32?__(42.0)
	push rax

	mov rax, rbp[-0x8]
	push rax

	pop rdi
	pop rax
	movd xmm0, eax
	call helper_foo_safe

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
