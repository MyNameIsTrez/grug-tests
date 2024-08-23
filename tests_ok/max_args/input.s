section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db "foo", 0

section .text

extern grug_block_mask

extern game_fn_define_d
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_mega
extern grug_disable_on_fn_runtime_error_handling
extern _GLOBAL_OFFSET_TABLE_

global define
define:
	call game_fn_define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret

%macro block 0
	xor edx, edx
	mov rsi, rbx[grug_block_mask wrt ..got]
	xor edi, edi
	call sigprocmask wrt ..plt
%endmacro

%macro unblock 0
	push rax
	xor edx, edx
	mov rsi, rbx[grug_block_mask wrt ..got]
	mov edi, 1
	call sigprocmask wrt ..plt
	pop rax
%endmacro

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rbx
	mov rbp[-0x10], rdi

	lea rbx, [rel $$]
	add rbx, _GLOBAL_OFFSET_TABLE_ wrt ..gotpc

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	block
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

	mov eax, 8192
	push rax

	lea rax, strings[rel 0]
	push rax

	; Popping arguments

	pop r9 ; "foo"

	pop r8 ; 8192

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
	unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret
