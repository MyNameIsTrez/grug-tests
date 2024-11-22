section .data

global define_type
define_type: db "w", 0

align 8
global globals_size
globals_size: dq 8

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

extern grug_runtime_error_handler
extern grug_runtime_error_jmp_buffer
extern grug_on_fns_in_safe_mode
extern grug_block_mask
extern grug_runtime_error_type
extern game_fn_define_w
extern __sigsetjmp
extern grug_get_runtime_error_reason
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_spawn
extern grug_disable_on_fn_runtime_error_handling

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

%macro error_handling 0
	mov esi, 1
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call __sigsetjmp wrt ..plt
	test eax, eax
	je strict $+0x33

	call grug_get_runtime_error_reason wrt ..plt
	mov rdi, rax

	lea rcx, strings[rel 59]

	lea rdx, strings[rel 120]

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
	mov rbp[-0xc], esi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict $+0x96

	error_handling

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	block
	lea rax, strings[rel 125]
	push rax
	pop rdi
	call game_fn_spawn wrt ..plt
	unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

	lea rax, strings[rel 125]
	push rax
	pop rdi
	call game_fn_spawn wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
