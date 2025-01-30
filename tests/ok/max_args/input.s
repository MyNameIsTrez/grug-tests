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
	db "tests/ok/max_args/input.grug", 0
on_fn_name:
	db "on_a", 0
	db "foo", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern grug_runtime_error_handler
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern game_fn_define_d
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_mega

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

	; Pushing arguments

	mov eax, __?float32?__(1.0)
	push rax

	mov eax, 21
	push rax

	mov eax, 1
	push rax

	mov eax, __?float32?__(2.0)
	push rax

	mov eax, __?float32?__(3.0)
	push rax

	mov eax, __?float32?__(4.0)
	push rax

	xor eax, eax
	push rax

	mov eax, 1337
	push rax

	mov eax, __?float32?__(5.0)
	push rax

	mov eax, __?float32?__(6.0)
	push rax

	mov eax, __?float32?__(7.0)
	push rax

	mov eax, __?float32?__(8.0)
	push rax

	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x0]
	push rax

	lea rax, strings[rel 34]
	push rax

	; Popping arguments

	pop r9 ; "foo"

	pop r8 ; me

	pop rax ; 8.0
	movd xmm7, eax

	pop rax ; 7.0
	movd xmm6, eax

	pop rax ; 6.0
	movd xmm5, eax

	pop rax ; 5.0
	movd xmm4, eax

	pop rcx ; 1337

	pop rdx ; false

	pop rax ; 4.0
	movd xmm3, eax

	pop rax ; 3.0
	movd xmm2, eax

	pop rax ; 2.0
	movd xmm1, eax

	pop rsi ; true

	pop rdi ; 21

	pop rax ; 1.0
	movd xmm0, eax

	; Calling function

	call game_fn_mega wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	; Pushing arguments

	mov eax, __?float32?__(1.0)
	push rax

	mov eax, 21
	push rax

	mov eax, 1
	push rax

	mov eax, __?float32?__(2.0)
	push rax

	mov eax, __?float32?__(3.0)
	push rax

	mov eax, __?float32?__(4.0)
	push rax

	xor eax, eax
	push rax

	mov eax, 1337
	push rax

	mov eax, __?float32?__(5.0)
	push rax

	mov eax, __?float32?__(6.0)
	push rax

	mov eax, __?float32?__(7.0)
	push rax

	mov eax, __?float32?__(8.0)
	push rax

	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x0]
	push rax

	lea rax, strings[rel 34]
	push rax

	; Popping arguments

	pop r9 ; "foo"

	pop r8 ; me

	pop rax ; 8.0
	movd xmm7, eax

	pop rax ; 7.0
	movd xmm6, eax

	pop rax ; 6.0
	movd xmm5, eax

	pop rax ; 5.0
	movd xmm4, eax

	pop rcx ; 1337

	pop rdx ; false

	pop rax ; 4.0
	movd xmm3, eax

	pop rax ; 3.0
	movd xmm2, eax

	pop rax ; 2.0
	movd xmm1, eax

	pop rsi ; true

	pop rdi ; 21

	pop rax ; 1.0
	movd xmm0, eax

	; Calling function

	call game_fn_mega wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
