section .data

global define_type
define_type: db "w", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db "tests/ok/resource_and_on_fn/foo.txt", 0
	db "tests/ok/resource_and_on_fn/input.grug", 0
	db "on_a", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq strings + 0

global entities_size
entities_size: dq 0

section .text

extern grug_on_fn_name
extern grug_on_fn_path

extern game_fn_define_w
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern grug_disable_on_fn_runtime_error_handling
extern _GLOBAL_OFFSET_TABLE_

global define
define:
	lea rax, strings[rel 0]
	mov rdi, rax
	call game_fn_define_w wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x20
	mov rbp[-0x8], rbx
	mov rbp[-0x10], rdi
	mov rbp[-0x14], esi

	lea rbx, [rel $$]
	add rbx, _GLOBAL_OFFSET_TABLE_ wrt ..gotpc

	lea rax, strings[rel 36]
	mov r11, rbx[grug_on_fn_path wrt ..got]
	mov [r11], rax

	lea rax, strings[rel 75]
	mov r11, rbx[grug_on_fn_name wrt ..got]
	mov [r11], rax

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret
