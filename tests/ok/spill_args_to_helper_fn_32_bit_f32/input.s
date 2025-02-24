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
	db "tests/ok/spill_args_to_helper_fn_32_bit_f32/input.grug", 0
on_fn_name:
	db "on_a", 0
s15:
	db "15", 0
s14:
	db "14", 0
s13:
	db "13", 0
s12:
	db "12", 0
s11:
	db "11", 0
s10:
	db "10", 0
s9:
	db "9", 0
s8:
	db "8", 0
s7:
	db "7", 0
s6:
	db "6", 0
s5:
	db "5", 0
s4:
	db "4", 0
s3:
	db "3", 0
s2:
	db "2", 0
s1:
	db "1", 0

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
extern game_fn_offset_32_bit_f32

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
	mov eax, 1
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

	lea rax, [rel s15]
	push rax
	lea rax, [rel s14]
	push rax
	lea rax, [rel s13]
	push rax
	lea rax, [rel s12]
	push rax
	lea rax, [rel s11]
	push rax
	lea rax, [rel s10]
	push rax
	lea rax, [rel s9]
	push rax
	lea rax, [rel s8]
	push rax
	lea rax, [rel s7]
	push rax
	lea rax, [rel s6]
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
	lea rax, [rel s5]
	push rax
	lea rax, [rel s4]
	push rax
	lea rax, [rel s3]
	push rax
	lea rax, [rel s2]
	push rax
	lea rax, [rel s1]
	push rax
	mov rax, rbp[-0x8]
	push rax

	pop rdi ; globals ptr
	pop rsi ; s1
	pop rdx ; s2
	pop rcx ; s3
	pop r8 ; s4
	pop r9 ; s5
	pop rax ; f1
	movd xmm0, eax
	pop rax ; f2
	movd xmm1, eax
	pop rax ; f3
	movd xmm2, eax
	pop rax ; f4
	movd xmm3, eax
	pop rax ; f5
	movd xmm4, eax
	pop rax ; f6
	movd xmm5, eax
	pop rax ; f7
	movd xmm6, eax
	pop rax ; f8
	movd xmm7, eax
	call helper_foo_safe
	add rsp, byte 0x50

	mov rsp, rbp
	pop rbp
	ret

.fast:
	lea rax, [rel s15]
	push rax
	lea rax, [rel s14]
	push rax
	lea rax, [rel s13]
	push rax
	lea rax, [rel s12]
	push rax
	lea rax, [rel s11]
	push rax
	lea rax, [rel s10]
	push rax
	lea rax, [rel s9]
	push rax
	lea rax, [rel s8]
	push rax
	lea rax, [rel s7]
	push rax
	lea rax, [rel s6]
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
	lea rax, [rel s5]
	push rax
	lea rax, [rel s4]
	push rax
	lea rax, [rel s3]
	push rax
	lea rax, [rel s2]
	push rax
	lea rax, [rel s1]
	push rax
	mov rax, rbp[-0x8]
	push rax

	pop rdi ; globals ptr
	pop rsi ; s1
	pop rdx ; s2
	pop rcx ; s3
	pop r8 ; s4
	pop r9 ; s5
	pop rax ; f1
	movd xmm0, eax
	pop rax ; f2
	movd xmm1, eax
	pop rax ; f3
	movd xmm2, eax
	pop rax ; f4
	movd xmm3, eax
	pop rax ; f5
	movd xmm4, eax
	pop rax ; f6
	movd xmm5, eax
	pop rax ; f7
	movd xmm6, eax
	pop rax ; f8
	movd xmm7, eax
	call helper_foo_fast
	add rsp, byte 0x50

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_safe
helper_foo_safe:
	push rbp
	mov rbp, rsp
	sub rsp, 0xa0
	mov rbp[-0x8], rdi ; globals pointer
	mov rbp[-0x10], rsi ; s1
	mov rbp[-0x18], rdx ; s2
	mov rbp[-0x20], rcx ; s3
	mov rbp[-0x28], r8 ; s4
	mov rbp[-0x30], r9 ; s5
	mov rax, rbp[dword +0x10] ; s6
	mov rbp[dword -0x38], rax
	mov rax, rbp[dword +0x18] ; s7
	mov rbp[dword -0x40], rax
	mov rax, rbp[dword +0x20] ; s8
	mov rbp[dword -0x48], rax
	mov rax, rbp[dword +0x28] ; s9
	mov rbp[dword -0x50], rax
	mov rax, rbp[dword +0x30] ; s10
	mov rbp[dword -0x58], rax
	mov rax, rbp[dword +0x38] ; s11
	mov rbp[dword -0x60], rax
	mov rax, rbp[dword +0x40] ; s12
	mov rbp[dword -0x68], rax
	mov rax, rbp[dword +0x48] ; s13
	mov rbp[dword -0x70], rax
	mov rax, rbp[dword +0x50] ; s14
	mov rbp[dword -0x78], rax
	mov rax, rbp[dword +0x58] ; s15
	mov rbp[dword -0x80], rax
	movss rbp[-0x84], xmm0 ; f1
	movss rbp[-0x88], xmm1 ; f2
	movss rbp[-0x8c], xmm2 ; f3
	movss rbp[-0x90], xmm3 ; f4
	movss rbp[-0x94], xmm4 ; f5
	movss rbp[-0x98], xmm5 ; f6
	movss rbp[-0x9c], xmm6 ; f7
	movss rbp[-0xa0], xmm7 ; f8
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	mov eax, rax[byte 0x8] ; global variable "g"
	push rax
	mov rax, rbp[-0x80] ; s15
    push rax
	mov rax, rbp[-0x78] ; s14
    push rax
	mov rax, rbp[-0x70] ; s13
    push rax
	mov rax, rbp[-0x68] ; s12
    push rax
	mov rax, rbp[-0x60] ; s11
    push rax
	mov rax, rbp[-0x58] ; s10
    push rax
	mov rax, rbp[-0x50] ; s9
    push rax
	mov rax, rbp[-0x48] ; s8
    push rax
	mov rax, rbp[-0x40] ; s7
    push rax

	mov eax, rbp[-0xa0] ; f8
    push rax
	mov eax, rbp[-0x9c] ; f7
    push rax
	mov eax, rbp[-0x98] ; f6
    push rax
	mov eax, rbp[-0x94] ; f5
    push rax
	mov eax, rbp[-0x90] ; f4
    push rax
	mov eax, rbp[-0x8c] ; f3
    push rax
	mov eax, rbp[-0x88] ; f2
    push rax
	mov eax, rbp[-0x84] ; f1
    push rax
	mov rax, rbp[-0x38] ; s6
    push rax
	mov rax, rbp[-0x30] ; s5
    push rax
	mov rax, rbp[-0x28] ; s4
    push rax
	mov rax, rbp[-0x20] ; s3
    push rax
	mov rax, rbp[-0x18] ; s2
    push rax
	mov rax, rbp[-0x10] ; s1
    push rax

	pop rdi ; s1
	pop rsi ; s2
	pop rdx ; s3
	pop rcx ; s4
	pop r8 ; s5
	pop r9 ; s6
	pop rax ; f1
	movd xmm0, eax
	pop rax ; f2
	movd xmm1, eax
	pop rax ; f3
	movd xmm2, eax
	pop rax ; f4
	movd xmm3, eax
	pop rax ; f5
	movd xmm4, eax
	pop rax ; f6
	movd xmm5, eax
	pop rax ; f7
	movd xmm6, eax
	pop rax ; f8
	movd xmm7, eax
	call game_fn_offset_32_bit_f32 wrt ..plt
	add rsp, byte 0x50

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_fast
helper_foo_fast:
	push rbp
	mov rbp, rsp
	sub rsp, 0xa0
	mov rbp[-0x8], rdi ; globals pointer
	mov rbp[-0x10], rsi ; s1
	mov rbp[-0x18], rdx ; s2
	mov rbp[-0x20], rcx ; s3
	mov rbp[-0x28], r8 ; s4
	mov rbp[-0x30], r9 ; s5
	mov rax, rbp[dword +0x10] ; s6
	mov rbp[dword -0x38], rax
	mov rax, rbp[dword +0x18] ; s7
	mov rbp[dword -0x40], rax
	mov rax, rbp[dword +0x20] ; s8
	mov rbp[dword -0x48], rax
	mov rax, rbp[dword +0x28] ; s9
	mov rbp[dword -0x50], rax
	mov rax, rbp[dword +0x30] ; s10
	mov rbp[dword -0x58], rax
	mov rax, rbp[dword +0x38] ; s11
	mov rbp[dword -0x60], rax
	mov rax, rbp[dword +0x40] ; s12
	mov rbp[dword -0x68], rax
	mov rax, rbp[dword +0x48] ; s13
	mov rbp[dword -0x70], rax
	mov rax, rbp[dword +0x50] ; s14
	mov rbp[dword -0x78], rax
	mov rax, rbp[dword +0x58] ; s15
	mov rbp[dword -0x80], rax
	movss rbp[-0x84], xmm0 ; f1
	movss rbp[-0x88], xmm1 ; f2
	movss rbp[-0x8c], xmm2 ; f3
	movss rbp[-0x90], xmm3 ; f4
	movss rbp[-0x94], xmm4 ; f5
	movss rbp[-0x98], xmm5 ; f6
	movss rbp[-0x9c], xmm6 ; f7
	movss rbp[-0xa0], xmm7 ; f8

	mov rax, rbp[-0x8]
	mov eax, rax[byte 0x8] ; global variable "g"
	push rax
	mov rax, rbp[-0x80] ; s15
    push rax
	mov rax, rbp[-0x78] ; s14
    push rax
	mov rax, rbp[-0x70] ; s13
    push rax
	mov rax, rbp[-0x68] ; s12
    push rax
	mov rax, rbp[-0x60] ; s11
    push rax
	mov rax, rbp[-0x58] ; s10
    push rax
	mov rax, rbp[-0x50] ; s9
    push rax
	mov rax, rbp[-0x48] ; s8
    push rax
	mov rax, rbp[-0x40] ; s7
    push rax

	mov eax, rbp[-0xa0] ; f8
    push rax
	mov eax, rbp[-0x9c] ; f7
    push rax
	mov eax, rbp[-0x98] ; f6
    push rax
	mov eax, rbp[-0x94] ; f5
    push rax
	mov eax, rbp[-0x90] ; f4
    push rax
	mov eax, rbp[-0x8c] ; f3
    push rax
	mov eax, rbp[-0x88] ; f2
    push rax
	mov eax, rbp[-0x84] ; f1
    push rax
	mov rax, rbp[-0x38] ; s6
    push rax
	mov rax, rbp[-0x30] ; s5
    push rax
	mov rax, rbp[-0x28] ; s4
    push rax
	mov rax, rbp[-0x20] ; s3
    push rax
	mov rax, rbp[-0x18] ; s2
    push rax
	mov rax, rbp[-0x10] ; s1
    push rax

	pop rdi ; s1
	pop rsi ; s2
	pop rdx ; s3
	pop rcx ; s4
	pop r8 ; s5
	pop r9 ; s6
	pop rax ; f1
	movd xmm0, eax
	pop rax ; f2
	movd xmm1, eax
	pop rax ; f3
	movd xmm2, eax
	pop rax ; f4
	movd xmm3, eax
	pop rax ; f5
	movd xmm4, eax
	pop rax ; f6
	movd xmm5, eax
	pop rax ; f7
	movd xmm6, eax
	pop rax ; f8
	movd xmm7, eax
	call game_fn_offset_32_bit_f32 wrt ..plt
	add rsp, byte 0x50

	mov rsp, rbp
	pop rbp
	ret
