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
	db 0
	db "tests/ok/entity_and_resource_as_subexpression/input.grug", 0
	db "on_a", 0
	db "tests/ok/entity_and_resource_as_subexpression/foo.txt", 0
	db "bar", 0
	db "ok:baz", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq strings + 63

global entities_size
entities_size: dq 1

global entities
entities:
	dq strings + 121

global entity_types
entity_types:
	dq strings + 0

section .text

extern grug_runtime_error_handler
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern game_fn_define_d
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_has_resource
extern game_fn_has_string
extern game_fn_has_entity
extern game_fn_initialize_bool

global define
define:
	sub rsp, byte 0x8
	call game_fn_define_d wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret

%macro save_on_fn_name_and_path 0
	mov rax, [rel grug_on_fn_path wrt ..got]
	lea r11, strings[rel 1]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, [rel on_fn_name]
	mov [rax], r11
%endmacro

%macro error_handling 0
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call setjmp wrt ..plt
	test eax, eax
	je %%skip

	dec eax
	push rax
	mov edi, eax
	sub rsp, byte 0x8
	call grug_get_runtime_error_reason wrt ..plt
	add rsp, byte 0x8
	mov rdi, rax

	lea rcx, [rel on_fn_path]

	lea rdx, [rel on_fn_name]

	pop rsi

	mov rax, [rel grug_runtime_error_handler wrt ..got]
	call [rax]

	mov rsp, rbp
	pop rbp
	ret
%%skip:
%endmacro

%macro block 0
	xor edx, edx
	mov rsi, [rel grug_block_mask wrt ..got]
	xor edi, edi
	call pthread_sigmask wrt ..plt
%endmacro

%macro unblock 0
	push rax
	xor edx, edx
	mov rsi, [rel grug_block_mask wrt ..got]
	mov edi, 1
	sub rsp, byte 0x8
	call pthread_sigmask wrt ..plt
	add rsp, byte 0x8
	pop rax
%endmacro

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict $+0x186

	save_on_fn_name_and_path

	error_handling

	lea rax, strings[rel 63]
	push rax
	pop rdi
	call game_fn_has_resource wrt ..plt

	; AND 1, part 1
	test eax, eax
	je strict $+0x4b

	lea rax, strings[rel 117]
	push rax
	pop rdi
	call game_fn_has_string wrt ..plt

	; AND 1, part 2
	test eax, eax
	mov eax, 0
	setne al

	; AND 2, part 1
	test eax, eax
	je strict $+0x4b

	lea rax, strings[rel 121]
	push rax
	pop rdi
	call game_fn_has_entity wrt ..plt

	; AND 2, part 2
	test eax, eax
	mov eax, 0
	setne al

	push rax

	pop rdi
	call game_fn_initialize_bool wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

	lea rax, strings[rel 63]
	push rax
	pop rdi
	call game_fn_has_resource wrt ..plt

	; AND 1, part 1
	test eax, eax
	je strict $+0x1e

	lea rax, strings[rel 117]
	push rax
	pop rdi
	call game_fn_has_string wrt ..plt

	; AND 1, part 2
	test eax, eax
	mov eax, 0
	setne al

	; AND 2, part 1
	test eax, eax
	je strict $+0x1e

	lea rax, strings[rel 121]
	push rax
	pop rdi
	call game_fn_has_entity wrt ..plt

	; AND 2, part 2
	test eax, eax
	mov eax, 0
	setne al

	push rax

	pop rdi
	call game_fn_initialize_bool wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
