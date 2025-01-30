section .data

global define_type
define_type: db "z", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

entity_type:
	db 0
entity:
	db "ok:foo", 0
on_fn_path:
	db "tests/ok/entity_and_on_fn/input.grug", 0
on_fn_name:
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 1

global entities
entities:
	dq entity

global entity_types
entity_types:
	dq entity_type

section .text

extern grug_runtime_error_handler
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern game_fn_define_z
extern setjmp
extern grug_get_runtime_error_reason

global define
define:
	sub rsp, byte 0x8
	lea rax, [rel entity]
	mov rdi, rax
	call game_fn_define_z wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret

%macro save_on_fn_name_and_path 0
	mov rax, [rel grug_on_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
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

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov rsp, rbp
	pop rbp
	ret
