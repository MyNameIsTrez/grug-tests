section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/sub_rsp_32_bits_local_variables_id/input-d.grug", 0
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

extern grug_call_runtime_error_handler
extern grug_fn_path
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern setjmp
extern game_fn_set_target
extern longjmp

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, 0x80
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_on_fn_name_and_path

	clear_has_runtime_error_happened

	mov rax, -1
	mov rbp[-0x10], rax
    mov rax, rbp[-0x10]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x18], rax
    mov rax, rbp[-0x18]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x20], rax
    mov rax, rbp[-0x20]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x28], rax
    mov rax, rbp[-0x28]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x30], rax
    mov rax, rbp[-0x30]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x38], rax
    mov rax, rbp[-0x38]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x40], rax
    mov rax, rbp[-0x40]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x48], rax
    mov rax, rbp[-0x48]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x50], rax
    mov rax, rbp[-0x50]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x58], rax
    mov rax, rbp[-0x58]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x60], rax
    mov rax, rbp[-0x60]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x68], rax
    mov rax, rbp[-0x68]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x70], rax
    mov rax, rbp[-0x70]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x78], rax
    mov rax, rbp[-0x78]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error
	mov rax, -1
	mov rbp[-0x80], rax
    mov rax, rbp[-0x80]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	check_game_fn_error

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov rax, -1
	mov rbp[-0x10], rax
    mov rax, rbp[-0x10]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x18], rax
    mov rax, rbp[-0x18]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x20], rax
    mov rax, rbp[-0x20]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x28], rax
    mov rax, rbp[-0x28]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x30], rax
    mov rax, rbp[-0x30]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x38], rax
    mov rax, rbp[-0x38]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x40], rax
    mov rax, rbp[-0x40]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x48], rax
    mov rax, rbp[-0x48]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x50], rax
    mov rax, rbp[-0x50]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x58], rax
    mov rax, rbp[-0x58]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x60], rax
    mov rax, rbp[-0x60]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x68], rax
    mov rax, rbp[-0x68]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x70], rax
    mov rax, rbp[-0x70]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x78], rax
    mov rax, rbp[-0x78]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, -1
	mov rbp[-0x80], rax
    mov rax, rbp[-0x80]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
