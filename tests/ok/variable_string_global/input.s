section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 16

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "temp", 0
on_fn_name:
	db "tests/ok/variable_string_global/input.grug", 0
	db "on_a", 0
	db "foo", 0

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
extern game_fn_say

global define
define:
	sub rsp, byte 0x8
	call game_fn_define_d wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	lea rax, strings[rel 0]
	mov rdi[0x8], rax
	ret

%macro save_on_fn_name_and_path 0
	mov rax, [rel grug_on_fn_path wrt ..got]
	lea r11, strings[rel 5]
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

	lea rax, strings[rel 53]
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], rax

	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x8]
	push rax

	pop rdi
	call game_fn_say wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	lea rax, strings[rel 53]
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], rax

	mov rax, rbp[-0x8]
	mov rax, rax[byte 0x8]
	push rax

	pop rdi
	call game_fn_say wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
