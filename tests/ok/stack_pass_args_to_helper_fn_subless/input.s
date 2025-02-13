section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 12

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/stack_pass_args_to_helper_fn_subless/input.grug", 0
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
extern game_fn_motherload_subless

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
	mov eax, 7
	mov rdi[0x8], eax
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

	mov eax, __?float32?__(10.0)
    push rax
	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x0]
    push rax
	mov eax, __?float32?__(9.0)
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
	add rsp, byte 0x20

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, __?float32?__(10.0)
    push rax
	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x0]
    push rax
	mov eax, __?float32?__(9.0)
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
	call helper_foo_fast
	add rsp, byte 0x20

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_safe
helper_foo_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x50
	mov rbp[-0x8], rdi ; globals pointer
	mov rbp[-0xc], esi ; 1
	mov rbp[-0x10], edx ; 2
	mov rbp[-0x14], ecx ; 3
	mov rbp[-0x18], r8d ; 4
	mov rbp[-0x1c], r9d ; 5
	mov eax, rbp[+0x10] ; 6
	mov rbp[-0x20], eax
	movss rbp[-0x24], xmm0 ; 1.0
	movss rbp[-0x28], xmm1 ; 2.0
	movss rbp[-0x2c], xmm2 ; 3.0
	movss rbp[-0x30], xmm3 ; 4.0
	movss rbp[-0x34], xmm4 ; 5.0
	movss rbp[-0x38], xmm5 ; 6.0
	movss rbp[-0x3c], xmm6 ; 7.0
	movss rbp[-0x40], xmm7 ; 8.0
	mov eax, rbp[+0x18] ; 9.0
	mov rbp[-0x44], eax
	mov rax, rbp[+0x20] ; me
	mov rbp[-0x4c], rax
	mov eax, rbp[+0x28] ; 10.0
	mov rbp[-0x50], eax
	check_stack_overflow

	mov eax, rbp[-0x50] ; 10.0
    push rax
	mov rax, rbp[-0x4c] ; me
    push rax
	mov eax, rbp[-0x44] ; 9.0
    push rax
	mov rax, rbp[-0x8]
	mov eax, rax[byte 0x8] ; global variable "g"
	push rax

	mov eax, rbp[-0x40] ; 8.0
    push rax
	mov eax, rbp[-0x3c] ; 7.0
    push rax
	mov eax, rbp[-0x38] ; 6.0
    push rax
	mov eax, rbp[-0x34] ; 5.0
    push rax
	mov eax, rbp[-0x30] ; 4.0
    push rax
	mov eax, rbp[-0x2c] ; 3.0
    push rax
	mov eax, rbp[-0x28] ; 2.0
    push rax
	mov eax, rbp[-0x24] ; 1.0
    push rax
	mov eax, rbp[-0x20] ; 6
    push rax
	mov eax, rbp[-0x1c] ; 5
    push rax
	mov eax, rbp[-0x18] ; 4
    push rax
	mov eax, rbp[-0x14] ; 3
    push rax
	mov eax, rbp[-0x10] ; 2
    push rax
	mov eax, rbp[-0xc] ; 1
    push rax

	pop rdi ; 1
	pop rsi ; 2
	pop rdx ; 3
	pop rcx ; 4
	pop r8 ; 5
	pop r9 ; 6
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
	call game_fn_motherload_subless wrt ..plt
	add rsp, byte 0x20

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_fast
helper_foo_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x50
	mov rbp[-0x8], rdi ; globals pointer
	mov rbp[-0xc], esi ; 1
	mov rbp[-0x10], edx ; 2
	mov rbp[-0x14], ecx ; 3
	mov rbp[-0x18], r8d ; 4
	mov rbp[-0x1c], r9d ; 5
	mov eax, rbp[+0x10] ; 6
	mov rbp[-0x20], eax
	movss rbp[-0x24], xmm0 ; 1.0
	movss rbp[-0x28], xmm1 ; 2.0
	movss rbp[-0x2c], xmm2 ; 3.0
	movss rbp[-0x30], xmm3 ; 4.0
	movss rbp[-0x34], xmm4 ; 5.0
	movss rbp[-0x38], xmm5 ; 6.0
	movss rbp[-0x3c], xmm6 ; 7.0
	movss rbp[-0x40], xmm7 ; 8.0
	mov eax, rbp[+0x18] ; 9.0
	mov rbp[-0x44], eax
	mov rax, rbp[+0x20] ; me
	mov rbp[-0x4c], rax
	mov eax, rbp[+0x28] ; 10.0
	mov rbp[-0x50], eax

	mov eax, rbp[-0x50] ; 10.0
    push rax
	mov rax, rbp[-0x4c] ; me
    push rax
	mov eax, rbp[-0x44] ; 9.0
    push rax
	mov rax, rbp[-0x8]
	mov eax, rax[byte 0x8] ; global variable "g"
	push rax

	mov eax, rbp[-0x40] ; 8.0
    push rax
	mov eax, rbp[-0x3c] ; 7.0
    push rax
	mov eax, rbp[-0x38] ; 6.0
    push rax
	mov eax, rbp[-0x34] ; 5.0
    push rax
	mov eax, rbp[-0x30] ; 4.0
    push rax
	mov eax, rbp[-0x2c] ; 3.0
    push rax
	mov eax, rbp[-0x28] ; 2.0
    push rax
	mov eax, rbp[-0x24] ; 1.0
    push rax
	mov eax, rbp[-0x20] ; 6
    push rax
	mov eax, rbp[-0x1c] ; 5
    push rax
	mov eax, rbp[-0x18] ; 4
    push rax
	mov eax, rbp[-0x14] ; 3
    push rax
	mov eax, rbp[-0x10] ; 2
    push rax
	mov eax, rbp[-0xc] ; 1
    push rax

	pop rdi ; 1
	pop rsi ; 2
	pop rdx ; 3
	pop rcx ; 4
	pop r8 ; 5
	pop r9 ; 6
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
	call game_fn_motherload_subless wrt ..plt
	add rsp, byte 0x20

	mov rsp, rbp
	pop rbp
	ret
