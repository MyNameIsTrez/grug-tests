section .data

global define_type
define_type: db "s", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db "tests/ok/on_fn_overwriting_param/input.grug", 0
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern grug_runtime_error_handler
extern grug_runtime_error_jmp_buffer
extern grug_block_mask
extern grug_runtime_error_type
extern game_fn_define_s
extern __sigsetjmp
extern grug_get_runtime_error_reason
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_initialize
extern game_fn_sin
extern grug_disable_on_fn_runtime_error_handling

global define
define:
	call game_fn_define_s wrt ..plt
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

	lea rcx, strings[rel 0]

	lea rdx, strings[rel 44]

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
	movss rbp[-0x10], xmm0

	error_handling

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	; i = 20
	mov eax, 20
	mov rbp[-0xc], eax

	; initialize(i)
	block
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	unblock

	; f = 30.0
	mov eax, __?float32?__(30.0)
	mov rbp[-0x10], eax

	block
	; sin(f)
	mov eax, rbp[-0x10]
	push rax
	pop rax
	movd xmm0, eax
	call game_fn_sin wrt ..plt
	movd eax, xmm0
	unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
