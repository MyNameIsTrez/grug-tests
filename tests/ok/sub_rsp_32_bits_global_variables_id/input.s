section .data

align 8
global globals_size
globals_size: dq 528

global on_fns
on_fns:
	dq on_a

init_globals_fn_path:
on_fn_path:
	db "tests/ok/sub_rsp_32_bits_global_variables_id/input-d.grug", 0
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
extern game_fn_get_opponent
extern game_fn_set_target

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

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x88], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x90], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x98], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xa0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xa8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xb0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xb8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xc0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xc8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xd0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xd8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xe0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xe8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xf0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xf8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x100], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x108], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x110], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x118], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x120], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x128], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x130], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x138], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x140], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x148], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x150], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x158], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x160], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x168], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x170], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x178], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x180], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x188], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x190], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x198], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1a0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1a8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1b0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1b8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1c0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1c8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1d0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1d8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1e0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1e8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1f0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1f8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x200], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x208], rax

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

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x88], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x90], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x98], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xa0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xa8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xb0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xb8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xc0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xc8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xd0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xd8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xe0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xe8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xf0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0xf8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x100], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x108], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x110], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x118], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x120], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x128], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x130], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x138], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x140], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x148], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x150], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x158], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x160], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x168], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x170], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x178], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x180], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x188], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x190], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x198], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1a0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1a8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1b0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1b8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1c0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1c8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1d0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1d8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1e0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1e8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1f0], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x1f8], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x200], rax

	call game_fn_get_opponent wrt ..plt
	mov r11, rbp[-0x8]
	mov r11[0x208], rax

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
	mov rax, rax[0x8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x10]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x18]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x20]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x28]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x30]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x38]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x40]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x48]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x50]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x58]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x60]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x68]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x70]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x78]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x80]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x88]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x90]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x98]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xa0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xa8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xb0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xb8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xc0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xc8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xd0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xd8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xe0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xe8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xf0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xf8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x100]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x108]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x110]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x118]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x120]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x128]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x130]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x138]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x140]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x148]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x150]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x158]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x160]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x168]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x170]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x178]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x180]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x188]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x190]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x198]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1a0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1a8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1b0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1b8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1c0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1c8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1d0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1d8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1e0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1e8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1f0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1f8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x200]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x208]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov rax, rbp[-0x8]
	mov rax, rax[0x8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x10]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x18]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x20]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x28]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x30]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x38]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x40]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x48]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x50]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x58]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x60]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x68]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x70]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x78]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x80]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x88]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x90]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x98]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xa0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xa8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xb0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xb8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xc0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xc8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xd0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xd8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xe0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xe8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xf0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0xf8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x100]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x108]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x110]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x118]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x120]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x128]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x130]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x138]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x140]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x148]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x150]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x158]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x160]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x168]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x170]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x178]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x180]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x188]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x190]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x198]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1a0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1a8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1b0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1b8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1c0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1c8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1d0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1d8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1e0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1e8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1f0]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x1f8]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x200]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt
	mov rax, rbp[-0x8]
	mov rax, rax[0x208]
	push rax
	pop rdi
	call game_fn_set_target wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
