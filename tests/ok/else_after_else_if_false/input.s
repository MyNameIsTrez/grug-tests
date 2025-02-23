section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/else_after_else_if_false/input.grug", 0
on_fn_name:
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
extern game_fn_define_d
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_nothing

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
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, [rel on_fn_name]
	mov [rax], r11
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
	je strict .fast

	save_on_fn_name_and_path

	call game_fn_nothing wrt ..plt

	xor eax, eax
	test eax, eax
	je strict .false
	call game_fn_nothing wrt ..plt
	jmp strict .end

.false:
	mov eax, 1
	test eax, eax
	je strict .false2
	jmp strict .end

.false2:
	call game_fn_nothing wrt ..plt

.end:
	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	call game_fn_nothing wrt ..plt

	xor eax, eax
	test eax, eax
	je strict .false_fast
	call game_fn_nothing wrt ..plt
	jmp strict .end_fast

.false_fast:
	mov eax, 1
	test eax, eax
	je strict .false2_fast
	jmp strict .end_fast

.false2_fast:
	call game_fn_nothing wrt ..plt

.end_fast:
	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
