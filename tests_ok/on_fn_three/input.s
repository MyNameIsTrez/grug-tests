section .data

global define_type
define_type: db "j", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a
	dq on_b
	dq on_c

global strings
strings:
	db "tests_ok/on_fn_three/input.grug", 0
	db "on_a", 0
	db "on_b", 0
	db "on_c", 0

section .text

extern grug_on_fn_name
extern grug_on_fn_path

extern game_fn_define_j
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern grug_disable_on_fn_runtime_error_handling
extern _GLOBAL_OFFSET_TABLE_

global define
define:
	call game_fn_define_j wrt ..plt
	ret

global init_globals
init_globals:
	ret

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

	lea rax, strings[rel 32]
	mov r11, rbx[grug_on_fn_name wrt ..got]
	mov [r11], rax

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret

global on_b
on_b:
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

	lea rax, strings[rel 37]
	mov r11, rbx[grug_on_fn_name wrt ..got]
	mov [r11], rax

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret

global on_c
on_c:
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

	lea rax, strings[rel 42]
	mov r11, rbx[grug_on_fn_name wrt ..got]
	mov [r11], rax

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret
