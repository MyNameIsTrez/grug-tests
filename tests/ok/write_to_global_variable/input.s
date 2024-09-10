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
	db "tests/ok/write_to_global_variable/input.grug", 0
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
extern sigprocmask
extern game_fn_max
extern grug_disable_on_fn_runtime_error_handling
extern _GLOBAL_OFFSET_TABLE_

global define
define:
	call game_fn_define_d wrt ..plt
	ret

global init_globals
init_globals:
	mov eax, 42
	mov rdi[byte 0x0], eax
	mov eax, 69
	mov rdi[0x4], eax
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

	lea rax, strings[rel 45]
	mov r11, rbx[grug_on_fn_name wrt ..got]
	mov [r11], rax

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	; foo + 1
	mov eax, 1
	push rax
	mov rax, rbp[-0x10]
	mov eax, rax[byte 0x0]
	pop r11
	add rax, r11

	; foo = foo + 1
	mov r11, rbp[-0x10]
	mov r11[byte 0x0], eax

	; push foo
	block
	mov rax, rbp[-0x10]
	mov eax, rax[byte 0x0]
	push rax

	; push bar
	mov rax, rbp[-0x10]
	mov eax, rax[0x4]
	push rax

	pop rsi
	pop rdi
	call game_fn_max wrt ..plt
	unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret
