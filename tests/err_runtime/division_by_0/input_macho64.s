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
	db "tests/err_runtime/division_by_0/input.grug", 0
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern grug_runtime_error_handler
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern grug_block_mask
extern grug_runtime_error_type
extern game_fn_define_d
extern __sigsetjmp
extern grug_get_runtime_error_reason
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_initialize_bool
extern grug_disable_on_fn_runtime_error_handling

global define
define:
	sub rsp, byte 0x8
	call game_fn_define_d wrt ..gotpcrel
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

	lea rcx, strings[rel 0]

	lea rdx, strings[rel ]

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

	; lea rbx, [rel $$]
	; add rbx, _GLOBAL_OFFSET_TABLE_ wrt ..gotpc

	; lea rax, strings[rel 0]
	; mov r11, [rel grug_on_fn_path wrt ..got]
	; mov [r11], rax

	; lea rax, strings[rel 43]
	; mov r11, [rel grug_on_fn_name wrt ..got]
	; mov [r11], rax

	; call grug_enable_on_fn_runtime_error_handling wrt ..gotpcrel

	; block
	; xor eax, eax
	; push rax
	; mov eax, 1
	; pop r11
	; cqo
	; idiv r11
	; push rax

	; pop rdi
	; call game_fn_initialize wrt ..gotpcrel
	; unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..gotpcrel

	mov rsp, rbp
	pop rbp
	ret
