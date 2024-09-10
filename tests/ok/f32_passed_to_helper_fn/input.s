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
	db "tests/ok/f32_passed_to_helper_fn/input.grug", 0
	db "on_a", 0

align 8
global dll_resources_size
dll_resources_size: dq 0

section .text

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_block_mask

extern game_fn_define_d
extern grug_enable_on_fn_runtime_error_handling
extern grug_disable_on_fn_runtime_error_handling
extern sigprocmask
extern _GLOBAL_OFFSET_TABLE_
extern game_fn_sin

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
	sub rsp, byte 0x10
	mov rbp[-0x8], rbx
	mov rbp[-0x10], rdi

	lea rbx, [rel $$]
	add rbx, _GLOBAL_OFFSET_TABLE_ wrt ..gotpc

	lea rax, strings[rel 0]
	mov r11, rbx[grug_on_fn_path wrt ..got]
	mov [r11], rax

	lea rax, strings[rel 44]
	mov r11, rbx[grug_on_fn_name wrt ..got]
	mov [r11], rax

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	mov rax, rbp[-0x10]
	push rax

	mov eax, __?float32?__(42.0)
	push rax

	pop rax
	movd xmm0, eax
	pop rdi
	call helper_foo

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret

global helper_foo
helper_foo:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x20
	mov rbp[-0x8], rbx
	mov rbp[-0x10], rdi
	movss rbp[-0x14], xmm0

	lea rbx, [rel $$]
	add rbx, _GLOBAL_OFFSET_TABLE_ wrt ..gotpc

	block
	mov eax, rbp[-0x14]
	push rax

	pop rax
	movd xmm0, eax
	call game_fn_sin wrt ..plt
	movd eax, xmm0
	unblock

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret
