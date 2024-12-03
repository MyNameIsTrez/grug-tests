section .data

global define_type
define_type: db "z", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db 0
	db "ok:foo", 0
	db "tests/ok/entity_and_on_fn/input.grug", 0
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 1

global entities
entities:
	dq strings + 1

global entity_types
entity_types:
	dq strings + 0

section .text

extern grug_runtime_error_handler
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern grug_block_mask
extern grug_runtime_error_type
extern game_fn_define_z
extern __sigsetjmp
extern grug_get_runtime_error_reason
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern grug_disable_on_fn_runtime_error_handling

global define
define:
	sub rsp, byte 0x8
	lea rax, strings[rel 1]
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
	lea r11, strings[rel 8]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, strings[rel 45]
	mov [rax], r11
%endmacro

%macro error_handling 0
	mov esi, 1
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call __sigsetjmp wrt ..plt
	test eax, eax
	je strict $+0x33

	call grug_get_runtime_error_reason wrt ..plt
	mov rdi, rax

	lea rcx, strings[rel 8]

	lea rdx, strings[rel 45]

	mov rsi, [rel grug_runtime_error_type wrt ..got]
	mov esi, [rsi]

	mov rax, [rel grug_runtime_error_handler wrt ..got]
	call [rax]

	mov rsp, rbp
	pop rbp
	ret
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
	je strict $+0x7d

	save_on_fn_name_and_path

	error_handling

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

	mov rsp, rbp
	pop rbp
	ret
