section .data

align 8
global globals_size
globals_size: dq 12

global on_fns
on_fns:
	dq on_a

init_globals_fn_path:
on_fn_path:
	db "tests/ok/on_fn_calling_no_game_fn_but_with_global/input-d.grug", 0
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

extern grug_call_runtime_error_handler
extern grug_fn_path
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern setjmp

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

	mov eax, 0x2a
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], eax

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 0x2a
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], eax

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

	mov eax, 1
	test al, al
	je strict .false_safe
.false_safe:

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 1
	test al, al
	je strict .false_fast
.false_fast:

	mov rsp, rbp
	pop rbp
	ret
