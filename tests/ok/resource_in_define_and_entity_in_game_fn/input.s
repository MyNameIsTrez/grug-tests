section .data

global define_type
define_type: db "w", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a
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

extern grug_runtime_error_handler
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern game_fn_define_w
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_spawn

global define
define:
	sub rsp, byte 0x8
	lea rax, strings[rel 1]
	mov rdi, rax
	call game_fn_define_w wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret

%macro save_on_fn_name_and_path 0
	mov rax, [rel grug_on_fn_path wrt ..got]
	lea r11, strings[rel 59]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, [rel on_fn_name]
	mov [rax], r11
%endmacro

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	mov rbp[-0xc], esi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_on_fn_name_and_path

	lea rax, strings[rel 125]
	push rax
	pop rdi
	call game_fn_spawn wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	lea rax, strings[rel 125]
	push rax
	pop rdi
	call game_fn_spawn wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
