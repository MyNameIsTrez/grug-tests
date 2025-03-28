section .data

align 8
global globals_size
globals_size: dq 16

init_globals_fn_path:
	db "tests/ok/global_id/input-a.grug", 0
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
extern grug_on_fns_in_safe_mode
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_get_opponent

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

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], rax

	mov rsp, rbp
	pop rbp
	ret

.fast:
	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], rax

	mov rsp, rbp
	pop rbp
	ret
