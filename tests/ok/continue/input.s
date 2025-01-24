section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db "tests/ok/continue/input.grug", 0
	db "on_a", 0

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
extern grug_block_mask
extern grug_runtime_error_type
extern game_fn_define_d
extern setjmp
extern grug_get_runtime_error_reason
extern grug_enable_on_fn_runtime_error_handling
extern pthread_sigmask
extern game_fn_nothing
extern grug_disable_on_fn_runtime_error_handling

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
	lea r11, strings[rel 0]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, strings[rel 29]
	mov [rax], r11
%endmacro

%macro error_handling 0
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call setjmp wrt ..plt
	test eax, eax
	je strict $+0x33

	call grug_get_runtime_error_reason wrt ..plt
	mov rdi, rax

	lea rcx, strings[rel 0]

	lea rdx, strings[rel 29]

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
	call pthread_sigmask wrt ..plt
%endmacro

%macro unblock 0
	push rax
	xor edx, edx
	mov rsi, [rel grug_block_mask wrt ..got]
	mov edi, 1
	sub rsp, byte 0x8
	call pthread_sigmask wrt ..plt
	add rsp, byte 0x8
	pop rax
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
	je strict $+0x11f

	save_on_fn_name_and_path

	error_handling

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	xor eax, eax
	mov rbp[-0xc], eax

	mov eax, 2
	push rax

	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11

	mov eax, 0x0
	setl al

	test eax, eax
	je strict $+0x85

	block
	call game_fn_nothing wrt ..plt
	unblock

	; i++
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	add rax, r11
	mov rbp[-0xc], eax

	jmp strict $-0x61 ; continue

	block
	call game_fn_nothing wrt ..plt
	unblock

	jmp strict $-0x98 ; jump to start of loop

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

	xor eax, eax
	mov rbp[-0xc], eax

	mov eax, 2
	push rax

	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11

	mov eax, 0x0
	setl al

	test eax, eax
	je strict $+0x2b

	call game_fn_nothing wrt ..plt

	; i++
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	add rax, r11
	mov rbp[-0xc], eax

	jmp strict $-0x34 ; continue

	call game_fn_nothing wrt ..plt

	jmp strict $-0x3e ; jump to start of loop

	mov rsp, rbp
	pop rbp
	ret
