section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/multiplication_as_two_arguments/input-d.grug", 0
on_fn_name:
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern grug_runtime_error_handler
extern grug_fn_path
extern grug_runtime_error_jmp_buffer
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern setjmp
extern grug_get_runtime_error_reason
extern longjmp
extern game_fn_max

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

	save_on_fn_name_and_path

	error_handling

	mov eax, 5
	push rax
	mov eax, 4
	pop r11
	imul r11d
	check_overflow
	push rax

	mov eax, 3
	push rax
	mov eax, 2
	pop r11
	imul r11d
	check_overflow
	push rax

	pop rdi
	pop rsi
	call game_fn_max wrt ..plt
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 5
	push rax
	mov eax, 4
	pop r11
	imul r11d
	push rax

	mov eax, 3
	push rax
	mov eax, 2
	pop r11
	imul r11d
	push rax

	pop rdi
	pop rsi
	call game_fn_max wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
