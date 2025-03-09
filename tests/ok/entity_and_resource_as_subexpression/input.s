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
	db "tests/ok/entity_and_resource_as_subexpression/input-d.grug", 0
on_fn_name:
	db "on_a", 0
resource:
	db "tests/ok/entity_and_resource_as_subexpression/foo.txt", 0
bar:
	db "bar", 0
entity:
	db "ok:baz", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq resource

global entities_size
entities_size: dq 1

global entities
entities:
	dq entity

global entity_types
entity_types:
	dq entity_type

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern grug_runtime_error_handler
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_has_resource
extern game_fn_has_string
extern game_fn_has_entity
extern game_fn_initialize_bool

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

	lea rax, [rel resource]
	push rax
	pop rdi
	call game_fn_has_resource wrt ..plt

	; AND 1, part 1
	test eax, eax
	je strict .false

	lea rax, [rel bar]
	push rax
	pop rdi
	call game_fn_has_string wrt ..plt

	; AND 1, part 2
	test eax, eax
	mov eax, 0
	setne al

	; AND 2, part 1
.false:
	test eax, eax
	je strict .false2

	lea rax, [rel entity]
	push rax
	pop rdi
	call game_fn_has_entity wrt ..plt

	; AND 2, part 2
	test eax, eax
	mov eax, 0
	setne al

.false2:
	push rax

	pop rdi
	call game_fn_initialize_bool wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	lea rax, [rel resource]
	push rax
	pop rdi
	call game_fn_has_resource wrt ..plt

	; AND 1, part 1
	test eax, eax
	je strict .false_fast

	lea rax, [rel bar]
	push rax
	pop rdi
	call game_fn_has_string wrt ..plt

	; AND 1, part 2
	test eax, eax
	mov eax, 0
	setne al

	; AND 2, part 1
.false_fast:
	test eax, eax
	je strict .false2_fast

	lea rax, [rel entity]
	push rax
	pop rdi
	call game_fn_has_entity wrt ..plt

	; AND 2, part 2
	test eax, eax
	mov eax, 0
	setne al

.false2_fast:
	push rax

	pop rdi
	call game_fn_initialize_bool wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
