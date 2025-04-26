section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/max_args/input-d.grug", 0
on_fn_name:
	db "on_a", 0
foo:
	db "foo", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern grug_runtime_error_handler
extern grug_fn_path
extern grug_runtime_error_jmp_buffer
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_mega
extern longjmp

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

	save_on_fn_name_and_path

	clear_has_runtime_error_happened

	; Pushing arguments

	lea rax, [rel foo]
	push rax

	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x0]
	push rax

	mov eax, __?float32?__(8.0)
	push rax

	mov eax, __?float32?__(7.0)
	push rax

	mov eax, __?float32?__(6.0)
	push rax

	mov eax, __?float32?__(5.0)
	push rax

	mov eax, 1337
	push rax

	xor eax, eax
	push rax

	mov eax, __?float32?__(4.0)
	push rax

	mov eax, __?float32?__(3.0)
	push rax

	mov eax, __?float32?__(2.0)
	push rax

	mov eax, 1
	push rax

	mov eax, 21
	push rax

	mov eax, __?float32?__(1.0)
	push rax

	; Popping arguments

	pop rax ; 1.0
	movd xmm0, eax
	pop rdi ; 21
	pop rsi ; true
	pop rax ; 2.0
	movd xmm1, eax
	pop rax ; 3.0
	movd xmm2, eax
	pop rax ; 4.0
	movd xmm3, eax
	pop rdx ; false
	pop rcx ; 1337
	pop rax ; 5.0
	movd xmm4, eax
	pop rax ; 6.0
	movd xmm5, eax
	pop rax ; 7.0
	movd xmm6, eax
	pop rax ; 8.0
	movd xmm7, eax
	pop r8 ; me
	pop r9 ; "foo"

	; Calling function

	call game_fn_mega wrt ..plt
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

.fast:
	; Pushing arguments

	lea rax, [rel foo]
	push rax

	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x0]
	push rax

	mov eax, __?float32?__(8.0)
	push rax

	mov eax, __?float32?__(7.0)
	push rax

	mov eax, __?float32?__(6.0)
	push rax

	mov eax, __?float32?__(5.0)
	push rax

	mov eax, 1337
	push rax

	xor eax, eax
	push rax

	mov eax, __?float32?__(4.0)
	push rax

	mov eax, __?float32?__(3.0)
	push rax

	mov eax, __?float32?__(2.0)
	push rax

	mov eax, 1
	push rax

	mov eax, 21
	push rax

	mov eax, __?float32?__(1.0)
	push rax

	; Popping arguments

	pop rax ; 1.0
	movd xmm0, eax
	pop rdi ; 21
	pop rsi ; true
	pop rax ; 2.0
	movd xmm1, eax
	pop rax ; 3.0
	movd xmm2, eax
	pop rax ; 4.0
	movd xmm3, eax
	pop rdx ; false
	pop rcx ; 1337
	pop rax ; 5.0
	movd xmm4, eax
	pop rax ; 6.0
	movd xmm5, eax
	pop rax ; 7.0
	movd xmm6, eax
	pop rax ; 8.0
	movd xmm7, eax
	pop r8 ; me
	pop r9 ; "foo"

	; Calling function

	call game_fn_mega wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
