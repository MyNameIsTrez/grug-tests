section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

section .text

extern grug_block_mask

extern game_fn_define_d
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_initialize
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

global on_a:
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
	mov rax, rbp[-0x8]
	push rax
	mov eax, 10
	push rax
	pop rsi
	pop rdi
	call helper_fib
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt	unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret

global helper_fib:
helper_fib:
	; Function prologue
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rbx
	mov rbp[-0x10], rdi
	mov rbp[-0xc], esi

	; if n == 0
	xor eax, eax
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11
	mov eax, 0
	sete al
	test eax, eax
	je $+0xc

	; or n == 1
	mov eax, 1
	jmp strict $+0x25
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11
	mov eax, 0
	sete al
	test eax, eax
	mov eax, 0
	setne al
	test eax, eax
	je strict $+0xe

	; return n
	mov eax, rbp[-0xc]
	mov rsp, rbp
	pop rbp
	ret

	; helper_fib(n - 2)
	mov rax, rbp[-0x8]
	push rax
	mov eax, 2
	push rax
	mov eax, rbp[-0xc]
	pop r11
	sub rax, r11
	push rax
	pop rsi
	pop rdi
	call helper_fib
	push rax

	; helper_fib(n - 1)
	mov rax, rbp[-0x8]
	push rax
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	sub rax, r11
	push rax
	pop rsi
	pop rdi
	sub rsp, byte 0x8
	call helper_fib
	add rsp, byte 0x8
	pop r11

	; helper_fib(n - 1) + helper_fib(n - 2)
	add rax, r11

	; return helper_fib(n - 1) + helper_fib(n - 2)
	mov rsp, rbp
	pop rbp
	ret

	; Function epilogue
	mov rsp, rbp
	pop rbp
	ret
