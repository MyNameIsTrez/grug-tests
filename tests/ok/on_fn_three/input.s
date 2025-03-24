section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a
	dq on_b
	dq on_c

on_fn_path:
	db "tests/ok/on_fn_three/input-j.grug", 0
on_fn_name_a:
	db "on_a", 0
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

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

%macro save_on_fn_name_and_path_on_a 0
	mov rax, [rel grug_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_fn_name wrt ..got]
	lea r11, [rel on_fn_name_a]
	mov [rax], r11
%endmacro

%macro save_on_fn_name_and_path_on_b 0
	mov rax, [rel grug_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_fn_name wrt ..got]
	lea r11, [rel on_fn_name_b]
	mov [rax], r11
%endmacro

%macro save_on_fn_name_and_path_on_c 0
	mov rax, [rel grug_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_fn_name wrt ..got]
	lea r11, [rel on_fn_name_c]
	mov [rax], r11
%endmacro

extern grug_runtime_error_handler
extern grug_fn_path
extern grug_runtime_error_jmp_buffer
extern grug_fn_name
extern grug_on_fns_in_safe_mode
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_initialize_bool
extern game_fn_nothing

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

	save_on_fn_name_and_path_on_a

	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

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

	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

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

	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
