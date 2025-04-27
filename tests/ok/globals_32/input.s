section .data

align 8
global globals_size
globals_size: dq 136

init_globals_fn_path:
	db "tests/ok/globals_32/input-a.grug", 0
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

	mov eax, 1
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], eax

	mov eax, 2
	mov r11, rbp[-0x8]
	mov r11[byte 0xc], eax

	mov eax, 3
	mov r11, rbp[-0x8]
	mov r11[byte 0x10], eax

	mov eax, 4
	mov r11, rbp[-0x8]
	mov r11[byte 0x14], eax

	mov eax, 5
	mov r11, rbp[-0x8]
	mov r11[byte 0x18], eax

	mov eax, 6
	mov r11, rbp[-0x8]
	mov r11[byte 0x1c], eax

	mov eax, 7
	mov r11, rbp[-0x8]
	mov r11[byte 0x20], eax

	mov eax, 8
	mov r11, rbp[-0x8]
	mov r11[byte 0x24], eax

	mov eax, 9
	mov r11, rbp[-0x8]
	mov r11[byte 0x28], eax

	mov eax, 10
	mov r11, rbp[-0x8]
	mov r11[byte 0x2c], eax

	mov eax, 11
	mov r11, rbp[-0x8]
	mov r11[byte 0x30], eax

	mov eax, 12
	mov r11, rbp[-0x8]
	mov r11[byte 0x34], eax

	mov eax, 13
	mov r11, rbp[-0x8]
	mov r11[byte 0x38], eax

	mov eax, 14
	mov r11, rbp[-0x8]
	mov r11[byte 0x3c], eax

	mov eax, 15
	mov r11, rbp[-0x8]
	mov r11[byte 0x40], eax

	mov eax, 16
	mov r11, rbp[-0x8]
	mov r11[byte 0x44], eax

	mov eax, 17
	mov r11, rbp[-0x8]
	mov r11[byte 0x48], eax

	mov eax, 18
	mov r11, rbp[-0x8]
	mov r11[byte 0x4c], eax

	mov eax, 19
	mov r11, rbp[-0x8]
	mov r11[byte 0x50], eax

	mov eax, 20
	mov r11, rbp[-0x8]
	mov r11[byte 0x54], eax

	mov eax, 21
	mov r11, rbp[-0x8]
	mov r11[byte 0x58], eax

	mov eax, 22
	mov r11, rbp[-0x8]
	mov r11[byte 0x5c], eax

	mov eax, 23
	mov r11, rbp[-0x8]
	mov r11[byte 0x60], eax

	mov eax, 24
	mov r11, rbp[-0x8]
	mov r11[byte 0x64], eax

	mov eax, 25
	mov r11, rbp[-0x8]
	mov r11[byte 0x68], eax

	mov eax, 26
	mov r11, rbp[-0x8]
	mov r11[byte 0x6c], eax

	mov eax, 27
	mov r11, rbp[-0x8]
	mov r11[byte 0x70], eax

	mov eax, 28
	mov r11, rbp[-0x8]
	mov r11[byte 0x74], eax

	mov eax, 29
	mov r11, rbp[-0x8]
	mov r11[byte 0x78], eax

	mov eax, 30
	mov r11, rbp[-0x8]
	mov r11[byte 0x7c], eax

	mov eax, 31
	mov r11, rbp[-0x8]
	mov r11[0x80], eax

	mov eax, 32
	mov r11, rbp[-0x8]
	mov r11[0x84], eax

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 1
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], eax

	mov eax, 2
	mov r11, rbp[-0x8]
	mov r11[byte 0xc], eax

	mov eax, 3
	mov r11, rbp[-0x8]
	mov r11[byte 0x10], eax

	mov eax, 4
	mov r11, rbp[-0x8]
	mov r11[byte 0x14], eax

	mov eax, 5
	mov r11, rbp[-0x8]
	mov r11[byte 0x18], eax

	mov eax, 6
	mov r11, rbp[-0x8]
	mov r11[byte 0x1c], eax

	mov eax, 7
	mov r11, rbp[-0x8]
	mov r11[byte 0x20], eax

	mov eax, 8
	mov r11, rbp[-0x8]
	mov r11[byte 0x24], eax

	mov eax, 9
	mov r11, rbp[-0x8]
	mov r11[byte 0x28], eax

	mov eax, 10
	mov r11, rbp[-0x8]
	mov r11[byte 0x2c], eax

	mov eax, 11
	mov r11, rbp[-0x8]
	mov r11[byte 0x30], eax

	mov eax, 12
	mov r11, rbp[-0x8]
	mov r11[byte 0x34], eax

	mov eax, 13
	mov r11, rbp[-0x8]
	mov r11[byte 0x38], eax

	mov eax, 14
	mov r11, rbp[-0x8]
	mov r11[byte 0x3c], eax

	mov eax, 15
	mov r11, rbp[-0x8]
	mov r11[byte 0x40], eax

	mov eax, 16
	mov r11, rbp[-0x8]
	mov r11[byte 0x44], eax

	mov eax, 17
	mov r11, rbp[-0x8]
	mov r11[byte 0x48], eax

	mov eax, 18
	mov r11, rbp[-0x8]
	mov r11[byte 0x4c], eax

	mov eax, 19
	mov r11, rbp[-0x8]
	mov r11[byte 0x50], eax

	mov eax, 20
	mov r11, rbp[-0x8]
	mov r11[byte 0x54], eax

	mov eax, 21
	mov r11, rbp[-0x8]
	mov r11[byte 0x58], eax

	mov eax, 22
	mov r11, rbp[-0x8]
	mov r11[byte 0x5c], eax

	mov eax, 23
	mov r11, rbp[-0x8]
	mov r11[byte 0x60], eax

	mov eax, 24
	mov r11, rbp[-0x8]
	mov r11[byte 0x64], eax

	mov eax, 25
	mov r11, rbp[-0x8]
	mov r11[byte 0x68], eax

	mov eax, 26
	mov r11, rbp[-0x8]
	mov r11[byte 0x6c], eax

	mov eax, 27
	mov r11, rbp[-0x8]
	mov r11[byte 0x70], eax

	mov eax, 28
	mov r11, rbp[-0x8]
	mov r11[byte 0x74], eax

	mov eax, 29
	mov r11, rbp[-0x8]
	mov r11[byte 0x78], eax

	mov eax, 30
	mov r11, rbp[-0x8]
	mov r11[byte 0x7c], eax

	mov eax, 31
	mov r11, rbp[-0x8]
	mov r11[0x80], eax

	mov eax, 32
	mov r11, rbp[-0x8]
	mov r11[0x84], eax

	mov rsp, rbp
	pop rbp
	ret
