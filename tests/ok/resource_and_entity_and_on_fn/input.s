section .data

global define_type
define_type: db "a2", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db 0
	db "ok:foo", 0
	db "tests/ok/resource_and_entity_and_on_fn/foo.txt", 0
	db "tests/ok/resource_and_entity_and_on_fn/input.grug", 0
	db "on_a", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq strings + 8

global entities_size
entities_size: dq 1

global entities
entities:
	dq strings + 1

global entity_types
entity_types:
	dq strings + 0

section .text

extern grug_on_fn_name
extern grug_on_fn_path

extern game_fn_define_a2
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern grug_disable_on_fn_runtime_error_handling
extern _GLOBAL_OFFSET_TABLE_

global define
define:
	lea rax, strings[rel 1]
	mov rsi, rax
	lea rax, strings[rel 8]
	mov rdi, rax
	call game_fn_define_a2 wrt ..plt
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

	lea rax, strings[rel 55]
	mov r11, rbx[grug_on_fn_path wrt ..got]
	mov [r11], rax

	lea rax, strings[rel 105]
	mov r11, rbx[grug_on_fn_name wrt ..got]
	mov [r11], rax

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret