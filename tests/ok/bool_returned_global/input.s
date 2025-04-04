section .data

align 8
global globals_size
globals_size: dq 9

global on_fns
on_fns:
	dq on_a

init_globals_fn_path:
on_fn_path:
	db "tests/ok/bool_returned_global/input-d.grug", 0
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

extern grug_runtime_error_handler
extern grug_fn_path
extern grug_runtime_error_jmp_buffer
extern grug_fn_name
extern grug_on_fns_in_safe_mode
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_initialize_bool
extern game_fn_get_evil_false
extern game_fn_set_is_happy

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

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], al

	mov rsp, rbp
	pop rbp
	ret

.fast:
	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], al

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

	call game_fn_get_evil_false wrt ..plt

	mov r11, rbp[-0x8]
	mov r11[0x8], al

	mov rax, rbp[-0x8]
	movzx eax, byte rax[0x8]
	push rax

	pop rdi
	call game_fn_set_is_happy wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	call game_fn_get_evil_false wrt ..plt

	mov r11, rbp[-0x8]
	mov r11[0x8], al

	mov rax, rbp[-0x8]
	movzx eax, byte rax[0x8]
	push rax

	pop rdi
	call game_fn_set_is_happy wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
