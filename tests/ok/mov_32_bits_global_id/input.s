section .data

align 8
global globals_size
globals_size: dq 0x88

init_globals_fn_path:
	db "tests/ok/mov_32_bits_global_id/input-a.grug", 0
init_globals_fn_name:
	db "init_globals", 0

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
extern game_fn_get_opponent
extern longjmp

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
	mov r11[0x8], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x10], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x18], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x20], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x28], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x30], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x38], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x40], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x48], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x50], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x58], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x60], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x68], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x70], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x78], rax

	call game_fn_get_opponent wrt ..plt
	check_game_fn_error
	mov r11, rbp[-0x8]
	mov r11[0x80], rax

	mov rsp, rbp
	pop rbp
	ret

.fast:
	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x10], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x18], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x20], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x28], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x30], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x38], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x40], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x48], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x50], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x58], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x60], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x68], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x70], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x78], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x80], rax

	mov rsp, rbp
	pop rbp
	ret
