section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

entity_type:
	db 0
on_fn_path:
	db "tests/ok/entity_twice/input-d.grug", 0
on_fn_name:
	db "on_a", 0
foo:
	db "ok:foo", 0
bar:
	db "ok:bar", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 2

global entities
entities:
	dq foo
	dq bar

global entity_types
entity_types:
	dq entity_type
	dq entity_type

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern game_fn_spawn

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_on_fn_name_and_path

	lea rax, [rel foo]
	push rax
	pop rdi
	call game_fn_spawn wrt ..plt

	lea rax, [rel bar]
	push rax
	pop rdi
	call game_fn_spawn wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	lea rax, [rel foo]
	push rax
	pop rdi
	call game_fn_spawn wrt ..plt

	lea rax, [rel bar]
	push rax
	pop rdi
	call game_fn_spawn wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
