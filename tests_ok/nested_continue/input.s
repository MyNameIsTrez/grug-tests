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
extern game_fn_nothing
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
	sub rsp, byte 0x8
	call sigprocmask wrt ..plt
	add rsp, byte 0x8
	pop rax
%endmacro

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x20
	mov rbp[-0x8], rbx
	mov rbp[-0x10], rdi

	lea rbx, [rel $$]
	add rbx, _GLOBAL_OFFSET_TABLE_ wrt ..gotpc

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	xor eax, eax
	mov rbp[-0x14], eax

	mov eax, 1
	test eax, eax
	je strict $+0xbb
	block
	call game_fn_nothing wrt ..plt
	unblock
	mov eax, 2
	push rax
	mov eax, rbp[-0x14]
	pop r11
	cmp rax, r11
	mov eax, 0
	setne al
	test eax, eax
	je strict $+0x69
	mov eax, 1
	push rax
	mov eax, rbp[-0x14]
	pop r11
	add rax, r11
	mov rbp[-0x14], eax
	mov eax, 1
	push rax
	mov eax, rbp[-0x14]
	pop r11
	cmp rax, r11
	mov eax, 0
	sete al
	test eax, eax
	je strict $+0xb
	jmp strict $-0x4d
	block
	call game_fn_nothing wrt ..plt
	unblock
	jmp strict $-0x7c
	jmp strict $+0xa
	jmp strict $-0xbd

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret
