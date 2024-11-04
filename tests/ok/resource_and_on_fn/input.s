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
	db "tests/ok/resource_and_on_fn/foo.txt", 0
	db "tests/ok/resource_and_on_fn/input.grug", 0
	db "on_a", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq strings + 0

global entities_size
entities_size: dq 0

section .text

extern grug_runtime_error_handler
extern grug_runtime_error_jmp_buffer
extern grug_block_mask
extern grug_runtime_error_type
extern game_fn_define_w
extern __sigsetjmp
extern grug_get_runtime_error_reason
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_initialize_bool
extern grug_disable_on_fn_runtime_error_handling

global define
define:
	lea rax, strings[rel 0]
	mov rdi, rax
	call game_fn_define_w wrt ..plt
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

	lea rcx, strings[rel 36]

	lea rdx, strings[rel 75]

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

	error_handling

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
