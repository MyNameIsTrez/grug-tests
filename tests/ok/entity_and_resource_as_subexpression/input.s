section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

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
extern grug_runtime_error_jmp_buffer
extern grug_block_mask
extern grug_runtime_error_type
extern game_fn_define_d
extern __sigsetjmp
extern grug_get_runtime_error_reason
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_has_resource
extern game_fn_has_string
extern game_fn_has_entity
extern game_fn_initialize_bool
extern grug_disable_on_fn_runtime_error_handling

global define
define:
	call game_fn_define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret

%macro error_handling 0
	mov esi, 1
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call __sigsetjmp wrt ..plt
	test eax, eax
	je strict $+0x33

	call grug_get_runtime_error_reason wrt ..plt
	mov rdi, rax

	lea rcx, strings[rel 1]

	lea rdx, strings[rel 58]

	mov rsi, [rel grug_runtime_error_type wrt ..got]
	mov esi, [rsi]

	mov rax, [rel grug_runtime_error_handler wrt ..got]
	call [rax]

	mov rsp, rbp
	pop rbp
	ret
%endmacro

%macro block 0
	xor edx, edx
	mov rsi, [rel grug_block_mask wrt ..got]
	xor edi, edi
	call sigprocmask wrt ..plt
%endmacro

%macro unblock 0
	push rax
	xor edx, edx
	mov rsi, [rel grug_block_mask wrt ..got]
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
	mov rbp[-0x8], rdi

	error_handling

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	block
	block
	lea rax, strings[rel 63]
	push rax
	pop rdi
	call game_fn_has_resource wrt ..plt
	unblock

	; AND 1, part 1
	test eax, eax
	je strict $+0x4b

	block
	lea rax, strings[rel 117]
	push rax
	pop rdi
	call game_fn_has_string wrt ..plt
	unblock

	; AND 1, part 2
	test eax, eax
	mov eax, 0
	setne al

	; AND 2, part 1
	test eax, eax
	je strict $+0x4b

	block
	lea rax, strings[rel 121]
	push rax
	pop rdi
	call game_fn_has_entity wrt ..plt
	unblock

	; AND 2, part 2
	test eax, eax
	mov eax, 0
	setne al

	push rax

	pop rdi
	call game_fn_initialize_bool wrt ..plt
	unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
