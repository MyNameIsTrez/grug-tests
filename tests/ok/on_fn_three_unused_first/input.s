section .data

global define_type
define_type: db "j", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq 0
	dq on_b
	dq on_c

on_fn_path:
	db "tests/ok/on_fn_three_unused_first/input.grug", 0
on_fn_name_b:
	db "on_b", 0
on_fn_name_c:
	db "on_c", 0

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
extern game_fn_define_j
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_initialize_bool

global define
define:
	sub rsp, byte 0x8
	call game_fn_define_j wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret

%macro save_on_fn_name_and_path_on_b 0
	mov rax, [rel grug_on_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, [rel on_fn_name_b]
	mov [rax], r11
%endmacro

global on_b
on_b:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_on_fn_name_and_path_on_b

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov rsp, rbp
	pop rbp
	ret

%macro save_on_fn_name_and_path_on_c 0
	mov rax, [rel grug_on_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, [rel on_fn_name_c]
	mov [rax], r11
%endmacro

global on_c
on_c:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_on_fn_name_and_path_on_c

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov rsp, rbp
	pop rbp
	ret
