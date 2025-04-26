section .data

align 8
global globals_size
globals_size: dq 12

global on_fns
on_fns:
	dq on_a

init_globals_fn_path:
on_fn_path:
	db "tests/ok/spill_args_to_helper_fn_32_bit_i32/input-d.grug", 0
init_globals_fn_name:
	db "init_globals", 0
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
extern setjmp
extern grug_get_runtime_error_reason
extern clock_gettime
extern longjmp
extern game_fn_offset_32_bit_i32

global init_globals
init_globals:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rdi[0x0], rsi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_init_globals_fn_name_and_path

	clear_has_runtime_error_happened

	mov eax, 6
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], eax

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 6
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], eax

	mov rsp, rbp
	pop rbp
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

	clear_has_runtime_error_happened

	mov eax, __?float32?__(30.0)
    push rax
	mov eax, __?float32?__(29.0)
    push rax
	mov eax, __?float32?__(28.0)
    push rax
	mov eax, __?float32?__(27.0)
    push rax
	mov eax, __?float32?__(26.0)
    push rax
	mov eax, __?float32?__(25.0)
    push rax
	mov eax, __?float32?__(24.0)
    push rax
	mov eax, __?float32?__(23.0)
    push rax
	mov eax, __?float32?__(22.0)
    push rax
	mov eax, __?float32?__(21.0)
    push rax
	mov eax, __?float32?__(20.0)
    push rax
	mov eax, __?float32?__(19.0)
    push rax
	mov eax, __?float32?__(18.0)
    push rax
	mov eax, __?float32?__(17.0)
    push rax
	mov eax, __?float32?__(16.0)
    push rax
	mov eax, __?float32?__(15.0)
    push rax
	mov eax, __?float32?__(14.0)
    push rax
	mov eax, __?float32?__(13.0)
    push rax
	mov eax, __?float32?__(12.0)
    push rax
	mov eax, __?float32?__(11.0)
    push rax
	mov eax, __?float32?__(10.0)
    push rax
	mov eax, __?float32?__(9.0)
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
	mov rax, rbp[-0x8]
	push rax

	pop rdi ; globals ptr
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
	pop rsi ; i1
	pop rdx ; i2
	pop rcx ; i3
	pop r8 ; i4
	pop r9 ; i5
	call helper_foo_safe
	add rsp, 0xb0

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, __?float32?__(30.0)
    push rax
	mov eax, __?float32?__(29.0)
    push rax
	mov eax, __?float32?__(28.0)
    push rax
	mov eax, __?float32?__(27.0)
    push rax
	mov eax, __?float32?__(26.0)
    push rax
	mov eax, __?float32?__(25.0)
    push rax
	mov eax, __?float32?__(24.0)
    push rax
	mov eax, __?float32?__(23.0)
    push rax
	mov eax, __?float32?__(22.0)
    push rax
	mov eax, __?float32?__(21.0)
    push rax
	mov eax, __?float32?__(20.0)
    push rax
	mov eax, __?float32?__(19.0)
    push rax
	mov eax, __?float32?__(18.0)
    push rax
	mov eax, __?float32?__(17.0)
    push rax
	mov eax, __?float32?__(16.0)
    push rax
	mov eax, __?float32?__(15.0)
    push rax
	mov eax, __?float32?__(14.0)
    push rax
	mov eax, __?float32?__(13.0)
    push rax
	mov eax, __?float32?__(12.0)
    push rax
	mov eax, __?float32?__(11.0)
    push rax
	mov eax, __?float32?__(10.0)
    push rax
	mov eax, __?float32?__(9.0)
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
	mov rax, rbp[-0x8]
	push rax

	pop rdi ; globals ptr
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
	pop rsi ; i1
	pop rdx ; i2
	pop rcx ; i3
	pop r8 ; i4
	pop r9 ; i5
	call helper_foo_fast
	add rsp, 0xb0

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_safe
helper_foo_safe:
	push rbp
	mov rbp, rsp
	sub rsp, 0xa0
	mov rbp[-0x8], rdi ; globals pointer
	movss rbp[-0xc], xmm0 ; f1
	movss rbp[-0x10], xmm1 ; f2
	movss rbp[-0x14], xmm2 ; f3
	movss rbp[-0x18], xmm3 ; f4
	movss rbp[-0x1c], xmm4 ; f5
	movss rbp[-0x20], xmm5 ; f6
	movss rbp[-0x24], xmm6 ; f7
	movss rbp[-0x28], xmm7 ; f8
	mov eax, rbp[dword +0x10] ; f9
	mov rbp[dword -0x2c], eax
	mov eax, rbp[dword +0x18] ; f10
	mov rbp[dword -0x30], eax
	mov eax, rbp[dword +0x20] ; f11
	mov rbp[dword -0x34], eax
	mov eax, rbp[dword +0x28] ; f12
	mov rbp[dword -0x38], eax
	mov eax, rbp[dword +0x30] ; f13
	mov rbp[dword -0x3c], eax
	mov eax, rbp[dword +0x38] ; f14
	mov rbp[dword -0x40], eax
	mov eax, rbp[dword +0x40] ; f15
	mov rbp[dword -0x44], eax
	mov eax, rbp[dword +0x48] ; f16
	mov rbp[dword -0x48], eax
	mov eax, rbp[dword +0x50] ; f17
	mov rbp[dword -0x4c], eax
	mov eax, rbp[dword +0x58] ; f18
	mov rbp[dword -0x50], eax
	mov eax, rbp[dword +0x60] ; f19
	mov rbp[dword -0x54], eax
	mov eax, rbp[dword +0x68] ; f20
	mov rbp[dword -0x58], eax
	mov eax, rbp[dword +0x70] ; f21
	mov rbp[dword -0x5c], eax
	mov eax, rbp[dword +0x78] ; f22
	mov rbp[dword -0x60], eax
	mov eax, rbp[dword +0x80] ; f23
	mov rbp[dword -0x64], eax
	mov eax, rbp[dword +0x88] ; f24
	mov rbp[dword -0x68], eax
	mov eax, rbp[dword +0x90] ; f25
	mov rbp[dword -0x6c], eax
	mov eax, rbp[dword +0x98] ; f26
	mov rbp[dword -0x70], eax
	mov eax, rbp[dword +0xa0] ; f27
	mov rbp[dword -0x74], eax
	mov eax, rbp[dword +0xa8] ; f28
	mov rbp[dword -0x78], eax
	mov eax, rbp[dword +0xb0] ; f29
	mov rbp[dword -0x7c], eax
	mov eax, rbp[dword +0xb8] ; f30
	mov rbp[dword -0x80], eax
	mov rbp[-0x84], esi ; i1
	mov rbp[-0x88], edx ; i2
	mov rbp[-0x8c], ecx ; i3
	mov rbp[-0x90], r8d ; i4
	mov rbp[-0x94], r9d ; i5
	check_stack_overflow
	check_time_limit_exceeded

	mov eax, rbp[-0x80] ; f30
    push rax
	mov eax, rbp[-0x7c] ; f29
    push rax
	mov eax, rbp[-0x78] ; f28
    push rax
	mov eax, rbp[-0x74] ; f27
    push rax
	mov eax, rbp[-0x70] ; f26
    push rax
	mov eax, rbp[-0x6c] ; f25
    push rax
	mov eax, rbp[-0x68] ; f24
    push rax
	mov eax, rbp[-0x64] ; f23
    push rax
	mov eax, rbp[-0x60] ; f22
    push rax
	mov eax, rbp[-0x5c] ; f21
    push rax
	mov eax, rbp[-0x58] ; f20
    push rax
	mov eax, rbp[-0x54] ; f19
    push rax
	mov eax, rbp[-0x50] ; f18
    push rax
	mov eax, rbp[-0x4c] ; f17
    push rax
	mov eax, rbp[-0x48] ; f16
    push rax
	mov eax, rbp[-0x44] ; f15
    push rax
	mov eax, rbp[-0x40] ; f14
    push rax
	mov eax, rbp[-0x3c] ; f13
    push rax
	mov eax, rbp[-0x38] ; f12
    push rax
	mov eax, rbp[-0x34] ; f11
    push rax
	mov eax, rbp[-0x30] ; f10
    push rax
	mov eax, rbp[-0x2c] ; f9
    push rax

	mov rax, rbp[-0x8]
	mov eax, rax[byte 0x8] ; global variable "g"
	push rax
	mov eax, rbp[-0x94] ; i5
    push rax
	mov eax, rbp[-0x90] ; i4
    push rax
	mov eax, rbp[-0x8c] ; i3
    push rax
	mov eax, rbp[-0x88] ; i2
    push rax
	mov eax, rbp[-0x84] ; i1
    push rax
	mov eax, rbp[-0x28] ; f8
    push rax
	mov eax, rbp[-0x24] ; f7
    push rax
	mov eax, rbp[-0x20] ; f6
    push rax
	mov eax, rbp[-0x1c] ; f5
    push rax
	mov eax, rbp[-0x18] ; f4
    push rax
	mov eax, rbp[-0x14] ; f3
    push rax
	mov eax, rbp[-0x10] ; f2
    push rax
	mov eax, rbp[-0xc] ; f1
    push rax

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
	pop rdi ; i1
	pop rsi ; i2
	pop rdx ; i3
	pop rcx ; i4
	pop r8 ; i5
	pop r9 ; global variable "g"
	call game_fn_offset_32_bit_i32 wrt ..plt
	add rsp, 0xb0
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_fast
helper_foo_fast:
	push rbp
	mov rbp, rsp
	sub rsp, 0xa0
	mov rbp[-0x8], rdi ; globals pointer
	movss rbp[-0xc], xmm0 ; f1
	movss rbp[-0x10], xmm1 ; f2
	movss rbp[-0x14], xmm2 ; f3
	movss rbp[-0x18], xmm3 ; f4
	movss rbp[-0x1c], xmm4 ; f5
	movss rbp[-0x20], xmm5 ; f6
	movss rbp[-0x24], xmm6 ; f7
	movss rbp[-0x28], xmm7 ; f8
	mov eax, rbp[dword +0x10] ; f9
	mov rbp[dword -0x2c], eax
	mov eax, rbp[dword +0x18] ; f10
	mov rbp[dword -0x30], eax
	mov eax, rbp[dword +0x20] ; f11
	mov rbp[dword -0x34], eax
	mov eax, rbp[dword +0x28] ; f12
	mov rbp[dword -0x38], eax
	mov eax, rbp[dword +0x30] ; f13
	mov rbp[dword -0x3c], eax
	mov eax, rbp[dword +0x38] ; f14
	mov rbp[dword -0x40], eax
	mov eax, rbp[dword +0x40] ; f15
	mov rbp[dword -0x44], eax
	mov eax, rbp[dword +0x48] ; f16
	mov rbp[dword -0x48], eax
	mov eax, rbp[dword +0x50] ; f17
	mov rbp[dword -0x4c], eax
	mov eax, rbp[dword +0x58] ; f18
	mov rbp[dword -0x50], eax
	mov eax, rbp[dword +0x60] ; f19
	mov rbp[dword -0x54], eax
	mov eax, rbp[dword +0x68] ; f20
	mov rbp[dword -0x58], eax
	mov eax, rbp[dword +0x70] ; f21
	mov rbp[dword -0x5c], eax
	mov eax, rbp[dword +0x78] ; f22
	mov rbp[dword -0x60], eax
	mov eax, rbp[dword +0x80] ; f23
	mov rbp[dword -0x64], eax
	mov eax, rbp[dword +0x88] ; f24
	mov rbp[dword -0x68], eax
	mov eax, rbp[dword +0x90] ; f25
	mov rbp[dword -0x6c], eax
	mov eax, rbp[dword +0x98] ; f26
	mov rbp[dword -0x70], eax
	mov eax, rbp[dword +0xa0] ; f27
	mov rbp[dword -0x74], eax
	mov eax, rbp[dword +0xa8] ; f28
	mov rbp[dword -0x78], eax
	mov eax, rbp[dword +0xb0] ; f29
	mov rbp[dword -0x7c], eax
	mov eax, rbp[dword +0xb8] ; f30
	mov rbp[dword -0x80], eax
	mov rbp[-0x84], esi ; i1
	mov rbp[-0x88], edx ; i2
	mov rbp[-0x8c], ecx ; i3
	mov rbp[-0x90], r8d ; i4
	mov rbp[-0x94], r9d ; i5

	mov eax, rbp[-0x80] ; f30
    push rax
	mov eax, rbp[-0x7c] ; f29
    push rax
	mov eax, rbp[-0x78] ; f28
    push rax
	mov eax, rbp[-0x74] ; f27
    push rax
	mov eax, rbp[-0x70] ; f26
    push rax
	mov eax, rbp[-0x6c] ; f25
    push rax
	mov eax, rbp[-0x68] ; f24
    push rax
	mov eax, rbp[-0x64] ; f23
    push rax
	mov eax, rbp[-0x60] ; f22
    push rax
	mov eax, rbp[-0x5c] ; f21
    push rax
	mov eax, rbp[-0x58] ; f20
    push rax
	mov eax, rbp[-0x54] ; f19
    push rax
	mov eax, rbp[-0x50] ; f18
    push rax
	mov eax, rbp[-0x4c] ; f17
    push rax
	mov eax, rbp[-0x48] ; f16
    push rax
	mov eax, rbp[-0x44] ; f15
    push rax
	mov eax, rbp[-0x40] ; f14
    push rax
	mov eax, rbp[-0x3c] ; f13
    push rax
	mov eax, rbp[-0x38] ; f12
    push rax
	mov eax, rbp[-0x34] ; f11
    push rax
	mov eax, rbp[-0x30] ; f10
    push rax
	mov eax, rbp[-0x2c] ; f9
    push rax

	mov rax, rbp[-0x8]
	mov eax, rax[byte 0x8] ; global variable "g"
	push rax
	mov eax, rbp[-0x94] ; i5
    push rax
	mov eax, rbp[-0x90] ; i4
    push rax
	mov eax, rbp[-0x8c] ; i3
    push rax
	mov eax, rbp[-0x88] ; i2
    push rax
	mov eax, rbp[-0x84] ; i1
    push rax
	mov eax, rbp[-0x28] ; f8
    push rax
	mov eax, rbp[-0x24] ; f7
    push rax
	mov eax, rbp[-0x20] ; f6
    push rax
	mov eax, rbp[-0x1c] ; f5
    push rax
	mov eax, rbp[-0x18] ; f4
    push rax
	mov eax, rbp[-0x14] ; f3
    push rax
	mov eax, rbp[-0x10] ; f2
    push rax
	mov eax, rbp[-0xc] ; f1
    push rax

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
	pop rdi ; i1
	pop rsi ; i2
	pop rdx ; i3
	pop rcx ; i4
	pop r8 ; i5
	pop r9 ; global variable "g"
	call game_fn_offset_32_bit_i32 wrt ..plt
	add rsp, 0xb0

	mov rsp, rbp
	pop rbp
	ret
