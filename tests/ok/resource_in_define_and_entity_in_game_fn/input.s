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
	db 0
	db "tests/ok/resource_in_define_and_entity_in_game_fn/foo.txt", 0
	db "tests/ok/resource_in_define_and_entity_in_game_fn/input.grug", 0
	db "on_a", 0
	db "ok:bar", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq strings + 1

global entities_size
entities_size: dq 1

global entities
entities:
	dq strings + 125

global entity_types
entity_types:
	dq strings + 0

section .text

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_block_mask

extern game_fn_define_w
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_spawn
extern grug_disable_on_fn_runtime_error_handling
extern _GLOBAL_OFFSET_TABLE_

global define
define:
	lea rax, strings[rel 1]
	mov rdi, rax
	call game_fn_define_w wrt ..plt
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
	mov rbp[-0x14], esi

	lea rbx, [rel $$]
	add rbx, _GLOBAL_OFFSET_TABLE_ wrt ..gotpc

	lea rax, strings[rel 59]
	mov r11, rbx[grug_on_fn_path wrt ..got]
	mov [r11], rax

	lea rax, strings[rel 120]
	mov r11, rbx[grug_on_fn_name wrt ..got]
	mov [r11], rax

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	block
	lea rax, strings[rel 125]
	push rax
	pop rdi
	call game_fn_spawn wrt ..plt
	unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret
