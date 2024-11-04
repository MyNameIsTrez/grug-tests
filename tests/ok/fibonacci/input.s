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
	db "tests/ok/fibonacci/input.grug", 0
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern grug_runtime_error_handler
extern grug_runtime_error_jmp_buffer
extern grug_block_mask
extern grug_runtime_error_type
extern game_fn_define_d
extern __sigsetjmp
extern grug_get_runtime_error_reason
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_initialize
extern grug_disable_on_fn_runtime_error_handling

global define
define:
	call game_fn_define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret

%macro error_handling 0
	mov esi, 1
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call __sigsetjmp wrt ..plt
	test eax, eax
	je strict $+0x33

	call grug_get_runtime_error_reason wrt ..plt
	mov rdi, rax

	lea rcx, strings[rel 0]

	lea rdx, strings[rel 30]

	mov rsi, [rel grug_runtime_error_type wrt ..got]
	mov esi, [rsi]

	mov rax, [rel grug_runtime_error_handler wrt ..got]
	call [rax]

	mov rsp, rbp
	pop rbp
	ret
%endmacro

%macro block 0
	xor edx, edx
	mov rsi, [rel grug_block_mask wrt ..got]
	xor edi, edi
	call sigprocmask wrt ..plt
%endmacro

%macro unblock 0
	push rax
	xor edx, edx
	mov rsi, [rel grug_block_mask wrt ..got]
	mov edi, 1
	sub rsp, byte 0x8
	call sigprocmask wrt ..plt
	add rsp, byte 0x8
	pop rax
%endmacro

global on_a:
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	error_handling

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
	call game_fn_initialize wrt ..plt
	unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

global helper_fib:
helper_fib:
	; Function prologue
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
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
