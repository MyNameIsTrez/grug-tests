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
	db "tests/ok/stack_pass_args_to_helper_fn/input.grug", 0
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
extern longjmp
extern game_fn_omega

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

	error_handling

	sub rsp, byte 0x8
	mov eax, 34
    push rax
	mov eax, 33
    push rax
	mov eax, 32
    push rax
	mov eax, 31
    push rax
	mov eax, 30
    push rax
	mov eax, 29
    push rax
	mov eax, 28
    push rax
	mov eax, 27
    push rax
	mov eax, 26
    push rax
	mov eax, 25
    push rax
	mov eax, 24
    push rax
	mov eax, 23
    push rax
	mov eax, 22
    push rax
	mov eax, 21
    push rax
	mov eax, 20
    push rax
	mov eax, 19
    push rax
	mov eax, 18
    push rax
	mov eax, 17
    push rax
	mov eax, 16
    push rax
	mov eax, 15
    push rax
	mov eax, 14
    push rax
	mov eax, 13
    push rax
	mov eax, 12
    push rax
	mov eax, 11
    push rax
	mov eax, 10
    push rax
	mov eax, 9
    push rax
	mov eax, __?float32?__(10.0)
    push rax
	mov eax, 8
    push rax
	mov eax, __?float32?__(9.0)
    push rax
	mov eax, 7
    push rax
	mov eax, 6
    push rax

	mov eax, __?float32?__(8.0)
    push rax
	mov eax, __?float32?__(7.0)
    push rax
	mov eax, __?float32?__(6.0)
    push rax
	mov eax, __?float32?__(5.0)
    push rax
	mov eax, __?float32?__(4.0)
    push rax
	mov eax, __?float32?__(3.0)
    push rax
	mov eax, __?float32?__(2.0)
    push rax
	mov eax, __?float32?__(1.0)
    push rax
	mov eax, 5
    push rax
	mov eax, 4
    push rax
	mov eax, 3
    push rax
	mov eax, 2
    push rax
	mov eax, 1
    push rax
	mov rax, rbp[-0x8]
	push rax

	pop rdi ; globals ptr
	pop rsi ; 1
	pop rdx ; 2
	pop rcx ; 3
	pop r8 ; 4
	pop r9 ; 5
	pop rax ; 1.0
	movd xmm0, eax
	pop rax ; 2.0
	movd xmm1, eax
	pop rax ; 3.0
	movd xmm2, eax
	pop rax ; 4.0
	movd xmm3, eax
	pop rax ; 5.0
	movd xmm4, eax
	pop rax ; 6.0
	movd xmm5, eax
	pop rax ; 7.0
	movd xmm6, eax
	pop rax ; 8.0
	movd xmm7, eax
	call helper_foo_safe
	add rsp, 0x100

	mov rsp, rbp
	pop rbp
	ret

.fast:
	sub rsp, byte 0x8
	mov eax, 34
    push rax
	mov eax, 33
    push rax
	mov eax, 32
    push rax
	mov eax, 31
    push rax
	mov eax, 30
    push rax
	mov eax, 29
    push rax
	mov eax, 28
    push rax
	mov eax, 27
    push rax
	mov eax, 26
    push rax
	mov eax, 25
    push rax
	mov eax, 24
    push rax
	mov eax, 23
    push rax
	mov eax, 22
    push rax
	mov eax, 21
    push rax
	mov eax, 20
    push rax
	mov eax, 19
    push rax
	mov eax, 18
    push rax
	mov eax, 17
    push rax
	mov eax, 16
    push rax
	mov eax, 15
    push rax
	mov eax, 14
    push rax
	mov eax, 13
    push rax
	mov eax, 12
    push rax
	mov eax, 11
    push rax
	mov eax, 10
    push rax
	mov eax, 9
    push rax
	mov eax, __?float32?__(10.0)
    push rax
	mov eax, 8
    push rax
	mov eax, __?float32?__(9.0)
    push rax
	mov eax, 7
    push rax
	mov eax, 6
    push rax

	mov eax, __?float32?__(8.0)
    push rax
	mov eax, __?float32?__(7.0)
    push rax
	mov eax, __?float32?__(6.0)
    push rax
	mov eax, __?float32?__(5.0)
    push rax
	mov eax, __?float32?__(4.0)
    push rax
	mov eax, __?float32?__(3.0)
    push rax
	mov eax, __?float32?__(2.0)
    push rax
	mov eax, __?float32?__(1.0)
    push rax
	mov eax, 5
    push rax
	mov eax, 4
    push rax
	mov eax, 3
    push rax
	mov eax, 2
    push rax
	mov eax, 1
    push rax
	mov rax, rbp[-0x8]
	push rax

	pop rdi ; globals ptr
	pop rsi ; 1
	pop rdx ; 2
	pop rcx ; 3
	pop r8 ; 4
	pop r9 ; 5
	pop rax ; 1.0
	movd xmm0, eax
	pop rax ; 2.0
	movd xmm1, eax
	pop rax ; 3.0
	movd xmm2, eax
	pop rax ; 4.0
	movd xmm3, eax
	pop rax ; 5.0
	movd xmm4, eax
	pop rax ; 6.0
	movd xmm5, eax
	pop rax ; 7.0
	movd xmm6, eax
	pop rax ; 8.0
	movd xmm7, eax
	call helper_foo_safe
	add rsp, 0x100

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

	call game_fn_omega wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_fast
helper_foo_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	call game_fn_omega wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
