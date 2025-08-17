section .data

align 8
global globals_size
globals_size: dq 24

global on_fns
on_fns:
	dq on_a

init_globals_fn_path:
on_fn_path:
	db "tests/ok/custom_id_transfer_between_globals/input-D.grug", 0
init_globals_fn_name:
	db "init_globals", 0
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
extern setjmp
extern game_fn_get_opponent
extern grug_call_runtime_error_handler
extern longjmp
extern game_fn_set_opponent

global init_globals
init_globals:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rdi[0x0], rsi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_init_globals_fn_name_and_path

	clear_has_runtime_error_happened

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error

	mov r11, rbp[-0x8]
	mov r11[byte 0x8], rax

	mov rax, rbp[-0x8]
	mov rax, rax[0x8]
	mov r11, rbp[-0x8]
	mov r11[0x10], rax

	mov rsp, rbp
	pop rbp
	ret

.fast:
	call game_fn_get_opponent wrt ..plt

	mov r11, rbp[-0x8]
	mov r11[byte 0x8], rax

	mov rax, rbp[-0x8]
	mov rax, rax[0x8]
	mov r11, rbp[-0x8]
	mov r11[0x10], rax

	mov rsp, rbp
	pop rbp
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

	mov rax, rbp[-0x8]
	mov rax, rax[0x10]
	push rax

	pop rdi
	call game_fn_set_opponent wrt ..plt
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov rax, rbp[-0x8]
	mov rax, rax[0x10]
	push rax

	pop rdi
	call game_fn_set_opponent wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
