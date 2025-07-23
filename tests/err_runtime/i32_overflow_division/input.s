section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/err_runtime/i32_overflow_division/input-d.grug", 0
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

extern grug_fn_path
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern grug_call_runtime_error_handler
extern game_fn_initialize

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

	clear_has_runtime_error_happened

	mov eax, 1
	neg eax
	check_overflow
	push rax
	mov eax, 1
	push rax
	mov eax, 2147483647
	neg eax
	check_overflow
	pop r11
	sub eax, r11d
	check_overflow
	pop r11
	check_division_by_0
	check_division_overflow
	cdq
	idiv r11d
	push rax

	pop rdi
	call game_fn_initialize wrt ..plt
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 1
	neg eax
	push rax
	mov eax, 1
	push rax
	mov eax, 2147483647
	neg eax
	pop r11
	sub eax, r11d
	pop r11
	cdq
	idiv r11d
	push rax

	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
