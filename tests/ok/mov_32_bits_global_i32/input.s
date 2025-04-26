section .data

align 8
global globals_size
globals_size: dq 0x84

init_globals_fn_path:
	db "tests/ok/mov_32_bits_global_i32/input-a.grug", 0
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

	init_globals_fn_error_handling

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xc], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x10], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x14], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x18], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x1c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x20], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x24], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x28], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x2c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x30], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x34], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x38], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x3c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x40], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x44], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x48], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x4c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x50], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x54], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x58], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x5c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x60], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x64], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x68], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x6c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x70], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x74], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x78], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x7c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x80], eax

	mov rsp, rbp
	pop rbp
	ret

.fast:
	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xc], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x10], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x14], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x18], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x1c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x20], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x24], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x28], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x2c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x30], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x34], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x38], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x3c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x40], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x44], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x48], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x4c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x50], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x54], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x58], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x5c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x60], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x64], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x68], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x6c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x70], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x74], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x78], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x7c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x80], eax

	mov rsp, rbp
	pop rbp
	ret
