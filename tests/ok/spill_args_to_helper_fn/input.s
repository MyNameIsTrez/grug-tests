section .data

align 8
global globals_size
globals_size: dq 12

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/spill_args_to_helper_fn/input-d.grug", 0
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
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern grug_current_time
extern clock_gettime
extern setjmp
extern grug_get_runtime_error_reason
extern longjmp
extern game_fn_motherload

global init_globals
init_globals:
	mov rdi[0x0], rsi
	mov eax, 7
	mov rdi[0x8], eax
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

	set_max_rsp

	set_time_limit

	error_handling

	sub rsp, byte 0x8
	mov eax, __?float32?__(9.0)
    push rax
	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x0]
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
	sub rsp, byte 0x8
	mov eax, __?float32?__(9.0)
    push rax
	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x0]
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
	mov eax, rbp[dword +0x10] ; 6
	mov rbp[dword -0x20], eax
	movss rbp[-0x24], xmm0 ; 1.0
	movss rbp[-0x28], xmm1 ; 2.0
	movss rbp[-0x2c], xmm2 ; 3.0
	movss rbp[-0x30], xmm3 ; 4.0
	movss rbp[-0x34], xmm4 ; 5.0
	movss rbp[-0x38], xmm5 ; 6.0
	movss rbp[-0x3c], xmm6 ; 7.0
	movss rbp[-0x40], xmm7 ; 8.0
	mov rax, rbp[dword +0x18] ; me
	mov rbp[dword -0x48], rax
	mov eax, rbp[dword +0x20] ; 9.0
	mov rbp[dword -0x4c], eax
	check_stack_overflow
	check_time_limit_exceeded

	sub rsp, byte 0x8
	mov eax, rbp[-0x4c] ; 9.0
    push rax
	mov rax, rbp[-0x48] ; me
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
	call game_fn_motherload wrt ..plt
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
	mov eax, rbp[dword +0x10] ; 6
	mov rbp[dword -0x20], eax
	movss rbp[-0x24], xmm0 ; 1.0
	movss rbp[-0x28], xmm1 ; 2.0
	movss rbp[-0x2c], xmm2 ; 3.0
	movss rbp[-0x30], xmm3 ; 4.0
	movss rbp[-0x34], xmm4 ; 5.0
	movss rbp[-0x38], xmm5 ; 6.0
	movss rbp[-0x3c], xmm6 ; 7.0
	movss rbp[-0x40], xmm7 ; 8.0
	mov rax, rbp[dword +0x18] ; me
	mov rbp[dword -0x48], rax
	mov eax, rbp[dword +0x20] ; 9.0
	mov rbp[dword -0x4c], eax

	sub rsp, byte 0x8
	mov eax, rbp[-0x4c] ; 9.0
    push rax
	mov rax, rbp[-0x48] ; me
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
	call game_fn_motherload wrt ..plt
	add rsp, byte 0x20

	mov rsp, rbp
	pop rbp
	ret
