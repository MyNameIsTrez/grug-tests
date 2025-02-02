section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 268

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/sub_rsp_32_bits_global_variables_i32/input.grug", 0
on_fn_name:
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern grug_runtime_error_handler
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern game_fn_define_d
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_initialize

global define
define:
	sub rsp, byte 0x8
	call game_fn_define_d wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	mov eax, 0x1
	mov [rdi + 0x8], eax
	mov eax, 0x2
	mov [rdi + 0xc], eax
	mov eax, 0x3
	mov [rdi + 0x10], eax
	mov eax, 0x4
	mov [rdi + 0x14], eax
	mov eax, 0x5
	mov [rdi + 0x18], eax
	mov eax, 0x6
	mov [rdi + 0x1c], eax
	mov eax, 0x7
	mov [rdi + 0x20], eax
	mov eax, 0x8
	mov [rdi + 0x24], eax
	mov eax, 0x9
	mov [rdi + 0x28], eax
	mov eax, 0xa
	mov [rdi + 0x2c], eax
	mov eax, 0xb
	mov [rdi + 0x30], eax
	mov eax, 0xc
	mov [rdi + 0x34], eax
	mov eax, 0xd
	mov [rdi + 0x38], eax
	mov eax, 0xe
	mov [rdi + 0x3c], eax
	mov eax, 0xf
	mov [rdi + 0x40], eax
	mov eax, 0x10
	mov [rdi + 0x44], eax
	mov eax, 0x11
	mov [rdi + 0x48], eax
	mov eax, 0x12
	mov [rdi + 0x4c], eax
	mov eax, 0x13
	mov [rdi + 0x50], eax
	mov eax, 0x14
	mov [rdi + 0x54], eax
	mov eax, 0x15
	mov [rdi + 0x58], eax
	mov eax, 0x16
	mov [rdi + 0x5c], eax
	mov eax, 0x17
	mov [rdi + 0x60], eax
	mov eax, 0x18
	mov [rdi + 0x64], eax
	mov eax, 0x19
	mov [rdi + 0x68], eax
	mov eax, 0x1a
	mov [rdi + 0x6c], eax
	mov eax, 0x1b
	mov [rdi + 0x70], eax
	mov eax, 0x1c
	mov [rdi + 0x74], eax
	mov eax, 0x1d
	mov [rdi + 0x78], eax
	mov eax, 0x1e
	mov [rdi + 0x7c], eax
	mov eax, 0x1f
	mov [rdi + 0x80], eax
	mov eax, 0x20
	mov [rdi + 0x84], eax
	mov eax, 0x21
	mov [rdi + 0x88], eax
	mov eax, 0x22
	mov [rdi + 0x8c], eax
	mov eax, 0x23
	mov [rdi + 0x90], eax
	mov eax, 0x24
	mov [rdi + 0x94], eax
	mov eax, 0x25
	mov [rdi + 0x98], eax
	mov eax, 0x26
	mov [rdi + 0x9c], eax
	mov eax, 0x27
	mov [rdi + 0xa0], eax
	mov eax, 0x28
	mov [rdi + 0xa4], eax
	mov eax, 0x29
	mov [rdi + 0xa8], eax
	mov eax, 0x2a
	mov [rdi + 0xac], eax
	mov eax, 0x2b
	mov [rdi + 0xb0], eax
	mov eax, 0x2c
	mov [rdi + 0xb4], eax
	mov eax, 0x2d
	mov [rdi + 0xb8], eax
	mov eax, 0x2e
	mov [rdi + 0xbc], eax
	mov eax, 0x2f
	mov [rdi + 0xc0], eax
	mov eax, 0x30
	mov [rdi + 0xc4], eax
	mov eax, 0x31
	mov [rdi + 0xc8], eax
	mov eax, 0x32
	mov [rdi + 0xcc], eax
	mov eax, 0x33
	mov [rdi + 0xd0], eax
	mov eax, 0x34
	mov [rdi + 0xd4], eax
	mov eax, 0x35
	mov [rdi + 0xd8], eax
	mov eax, 0x36
	mov [rdi + 0xdc], eax
	mov eax, 0x37
	mov [rdi + 0xe0], eax
	mov eax, 0x38
	mov [rdi + 0xe4], eax
	mov eax, 0x39
	mov [rdi + 0xe8], eax
	mov eax, 0x3a
	mov [rdi + 0xec], eax
	mov eax, 0x3b
	mov [rdi + 0xf0], eax
	mov eax, 0x3c
	mov [rdi + 0xf4], eax
	mov eax, 0x3d
	mov [rdi + 0xf8], eax
	mov eax, 0x3e
	mov [rdi + 0xfc], eax
	mov eax, 0x3f
	mov [rdi + 0x100], eax
	mov eax, 0x40
	mov [rdi + 0x104], eax
	mov eax, 0x41
	mov [rdi + 0x108], eax
	ret

%macro save_on_fn_name_and_path 0
	mov rax, [rel grug_on_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, [rel on_fn_name]
	mov [rax], r11
%endmacro

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
