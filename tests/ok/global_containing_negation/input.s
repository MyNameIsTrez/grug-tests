section .data

align 8
global globals_size
globals_size: dq 12

init_globals_fn_path:
	db "tests/ok/global_containing_negation/input-a.grug", 0
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

	mov eax, 2
	neg eax
	check_overflow
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], eax

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 2
	neg eax
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], eax

	mov rsp, rbp
	pop rbp
	ret
