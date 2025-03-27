section .data

align 8
global globals_size
globals_size: dq 268

global on_fns
on_fns:
	dq on_a

init_globals_fn_path:
on_fn_path:
	db "tests/ok/sub_rsp_32_bits_global_variables_i32/input-d.grug", 0
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
extern game_fn_initialize

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

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x84], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x88], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x8c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x90], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x94], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x98], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x9c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xa0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xa4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xa8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xac], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xb0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xb4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xb8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xbc], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xc0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xc4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xc8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xcc], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xd0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xd4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xd8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xdc], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xe0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xe4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xe8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xec], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xf0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xf4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xf8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xfc], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x100], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x104], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x108], eax

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

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x84], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x88], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x8c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x90], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x94], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x98], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x9c], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xa0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xa4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xa8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xac], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xb0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xb4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xb8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xbc], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xc0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xc4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xc8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xcc], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xd0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xd4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xd8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xdc], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xe0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xe4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xe8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xec], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xf0], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xf4], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xf8], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0xfc], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x100], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x104], eax

	xor eax, eax
	mov r11, rbp[-0x8]
	mov r11[0x108], eax

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

	mov rax, rbp[-0x8]
	mov eax, rax[0x8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x10]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x14]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x18]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x1c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x20]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x24]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x28]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x2c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x30]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x34]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x38]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x3c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x40]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x44]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x48]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x4c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x50]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x54]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x58]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x5c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x60]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x64]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x68]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x6c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x70]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x74]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x78]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x7c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x80]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x84]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x88]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x8c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x90]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x94]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x98]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x9c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xa0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xa4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xa8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xac]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xb0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xb4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xb8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xbc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xc0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xc4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xc8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xcc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xd0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xd4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xd8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xdc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xe0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xe4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xe8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xec]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xf0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xf4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xf8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xfc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x100]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x104]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x108]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov rax, rbp[-0x8]
	mov eax, rax[0x8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x10]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x14]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x18]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x1c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x20]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x24]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x28]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x2c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x30]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x34]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x38]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x3c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x40]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x44]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x48]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x4c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x50]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x54]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x58]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x5c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x60]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x64]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x68]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x6c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x70]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x74]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x78]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x7c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x80]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x84]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x88]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x8c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x90]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x94]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x98]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x9c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xa0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xa4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xa8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xac]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xb0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xb4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xb8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xbc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xc0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xc4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xc8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xcc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xd0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xd4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xd8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xdc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xe0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xe4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xe8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xec]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xf0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xf4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xf8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0xfc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x100]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x104]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov rax, rbp[-0x8]
	mov eax, rax[0x108]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
