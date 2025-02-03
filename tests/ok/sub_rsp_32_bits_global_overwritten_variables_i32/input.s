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
	db "tests/ok/sub_rsp_32_bits_global_overwritten_variables_i32/input.grug", 0
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
	xor eax, eax
	mov [rdi + 0x8], eax
	xor eax, eax
	mov [rdi + 0xc], eax
	xor eax, eax
	mov [rdi + 0x10], eax
	xor eax, eax
	mov [rdi + 0x14], eax
	xor eax, eax
	mov [rdi + 0x18], eax
	xor eax, eax
	mov [rdi + 0x1c], eax
	xor eax, eax
	mov [rdi + 0x20], eax
	xor eax, eax
	mov [rdi + 0x24], eax
	xor eax, eax
	mov [rdi + 0x28], eax
	xor eax, eax
	mov [rdi + 0x2c], eax
	xor eax, eax
	mov [rdi + 0x30], eax
	xor eax, eax
	mov [rdi + 0x34], eax
	xor eax, eax
	mov [rdi + 0x38], eax
	xor eax, eax
	mov [rdi + 0x3c], eax
	xor eax, eax
	mov [rdi + 0x40], eax
	xor eax, eax
	mov [rdi + 0x44], eax
	xor eax, eax
	mov [rdi + 0x48], eax
	xor eax, eax
	mov [rdi + 0x4c], eax
	xor eax, eax
	mov [rdi + 0x50], eax
	xor eax, eax
	mov [rdi + 0x54], eax
	xor eax, eax
	mov [rdi + 0x58], eax
	xor eax, eax
	mov [rdi + 0x5c], eax
	xor eax, eax
	mov [rdi + 0x60], eax
	xor eax, eax
	mov [rdi + 0x64], eax
	xor eax, eax
	mov [rdi + 0x68], eax
	xor eax, eax
	mov [rdi + 0x6c], eax
	xor eax, eax
	mov [rdi + 0x70], eax
	xor eax, eax
	mov [rdi + 0x74], eax
	xor eax, eax
	mov [rdi + 0x78], eax
	xor eax, eax
	mov [rdi + 0x7c], eax
	xor eax, eax
	mov [rdi + 0x80], eax
	xor eax, eax
	mov [rdi + 0x84], eax
	xor eax, eax
	mov [rdi + 0x88], eax
	xor eax, eax
	mov [rdi + 0x8c], eax
	xor eax, eax
	mov [rdi + 0x90], eax
	xor eax, eax
	mov [rdi + 0x94], eax
	xor eax, eax
	mov [rdi + 0x98], eax
	xor eax, eax
	mov [rdi + 0x9c], eax
	xor eax, eax
	mov [rdi + 0xa0], eax
	xor eax, eax
	mov [rdi + 0xa4], eax
	xor eax, eax
	mov [rdi + 0xa8], eax
	xor eax, eax
	mov [rdi + 0xac], eax
	xor eax, eax
	mov [rdi + 0xb0], eax
	xor eax, eax
	mov [rdi + 0xb4], eax
	xor eax, eax
	mov [rdi + 0xb8], eax
	xor eax, eax
	mov [rdi + 0xbc], eax
	xor eax, eax
	mov [rdi + 0xc0], eax
	xor eax, eax
	mov [rdi + 0xc4], eax
	xor eax, eax
	mov [rdi + 0xc8], eax
	xor eax, eax
	mov [rdi + 0xcc], eax
	xor eax, eax
	mov [rdi + 0xd0], eax
	xor eax, eax
	mov [rdi + 0xd4], eax
	xor eax, eax
	mov [rdi + 0xd8], eax
	xor eax, eax
	mov [rdi + 0xdc], eax
	xor eax, eax
	mov [rdi + 0xe0], eax
	xor eax, eax
	mov [rdi + 0xe4], eax
	xor eax, eax
	mov [rdi + 0xe8], eax
	xor eax, eax
	mov [rdi + 0xec], eax
	xor eax, eax
	mov [rdi + 0xf0], eax
	xor eax, eax
	mov [rdi + 0xf4], eax
	xor eax, eax
	mov [rdi + 0xf8], eax
	xor eax, eax
	mov [rdi + 0xfc], eax
	xor eax, eax
	mov [rdi + 0x100], eax
	xor eax, eax
	mov [rdi + 0x104], eax
	xor eax, eax
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

	mov eax, 1
	mov r11, rbp[-0x8]
	mov r11[0x8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 2
	mov r11, rbp[-0x8]
	mov r11[0xc], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 3
	mov r11, rbp[-0x8]
	mov r11[0x10], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x10]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 4
	mov r11, rbp[-0x8]
	mov r11[0x14], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x14]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 5
	mov r11, rbp[-0x8]
	mov r11[0x18], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x18]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 6
	mov r11, rbp[-0x8]
	mov r11[0x1c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x1c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 7
	mov r11, rbp[-0x8]
	mov r11[0x20], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x20]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 8
	mov r11, rbp[-0x8]
	mov r11[0x24], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x24]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 9
	mov r11, rbp[-0x8]
	mov r11[0x28], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x28]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 10
	mov r11, rbp[-0x8]
	mov r11[0x2c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x2c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 11
	mov r11, rbp[-0x8]
	mov r11[0x30], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x30]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 12
	mov r11, rbp[-0x8]
	mov r11[0x34], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x34]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 13
	mov r11, rbp[-0x8]
	mov r11[0x38], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x38]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 14
	mov r11, rbp[-0x8]
	mov r11[0x3c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x3c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 15
	mov r11, rbp[-0x8]
	mov r11[0x40], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x40]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 16
	mov r11, rbp[-0x8]
	mov r11[0x44], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x44]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 17
	mov r11, rbp[-0x8]
	mov r11[0x48], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x48]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 18
	mov r11, rbp[-0x8]
	mov r11[0x4c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x4c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 19
	mov r11, rbp[-0x8]
	mov r11[0x50], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x50]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 20
	mov r11, rbp[-0x8]
	mov r11[0x54], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x54]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 21
	mov r11, rbp[-0x8]
	mov r11[0x58], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x58]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 22
	mov r11, rbp[-0x8]
	mov r11[0x5c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x5c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 23
	mov r11, rbp[-0x8]
	mov r11[0x60], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x60]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 24
	mov r11, rbp[-0x8]
	mov r11[0x64], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x64]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 25
	mov r11, rbp[-0x8]
	mov r11[0x68], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x68]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 26
	mov r11, rbp[-0x8]
	mov r11[0x6c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x6c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 27
	mov r11, rbp[-0x8]
	mov r11[0x70], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x70]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 28
	mov r11, rbp[-0x8]
	mov r11[0x74], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x74]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 29
	mov r11, rbp[-0x8]
	mov r11[0x78], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x78]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 30
	mov r11, rbp[-0x8]
	mov r11[0x7c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x7c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 31
	mov r11, rbp[-0x8]
	mov r11[0x80], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x80]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 32
	mov r11, rbp[-0x8]
	mov r11[0x84], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x84]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 33
	mov r11, rbp[-0x8]
	mov r11[0x88], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x88]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 34
	mov r11, rbp[-0x8]
	mov r11[0x8c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x8c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 35
	mov r11, rbp[-0x8]
	mov r11[0x90], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x90]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 36
	mov r11, rbp[-0x8]
	mov r11[0x94], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x94]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 37
	mov r11, rbp[-0x8]
	mov r11[0x98], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x98]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 38
	mov r11, rbp[-0x8]
	mov r11[0x9c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x9c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 39
	mov r11, rbp[-0x8]
	mov r11[0xa0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xa0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 40
	mov r11, rbp[-0x8]
	mov r11[0xa4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xa4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 41
	mov r11, rbp[-0x8]
	mov r11[0xa8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xa8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 42
	mov r11, rbp[-0x8]
	mov r11[0xac], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xac]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 43
	mov r11, rbp[-0x8]
	mov r11[0xb0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xb0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 44
	mov r11, rbp[-0x8]
	mov r11[0xb4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xb4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 45
	mov r11, rbp[-0x8]
	mov r11[0xb8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xb8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 46
	mov r11, rbp[-0x8]
	mov r11[0xbc], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xbc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 47
	mov r11, rbp[-0x8]
	mov r11[0xc0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xc0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 48
	mov r11, rbp[-0x8]
	mov r11[0xc4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xc4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 49
	mov r11, rbp[-0x8]
	mov r11[0xc8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xc8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 50
	mov r11, rbp[-0x8]
	mov r11[0xcc], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xcc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 51
	mov r11, rbp[-0x8]
	mov r11[0xd0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xd0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 52
	mov r11, rbp[-0x8]
	mov r11[0xd4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xd4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 53
	mov r11, rbp[-0x8]
	mov r11[0xd8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xd8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 54
	mov r11, rbp[-0x8]
	mov r11[0xdc], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xdc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 55
	mov r11, rbp[-0x8]
	mov r11[0xe0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xe0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 56
	mov r11, rbp[-0x8]
	mov r11[0xe4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xe4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 57
	mov r11, rbp[-0x8]
	mov r11[0xe8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xe8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 58
	mov r11, rbp[-0x8]
	mov r11[0xec], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xec]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 59
	mov r11, rbp[-0x8]
	mov r11[0xf0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xf0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 60
	mov r11, rbp[-0x8]
	mov r11[0xf4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xf4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 61
	mov r11, rbp[-0x8]
	mov r11[0xf8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xf8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 62
	mov r11, rbp[-0x8]
	mov r11[0xfc], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xfc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 63
	mov r11, rbp[-0x8]
	mov r11[0x100], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x100]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 64
	mov r11, rbp[-0x8]
	mov r11[0x104], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x104]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 65
	mov r11, rbp[-0x8]
	mov r11[0x108], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x108]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 1
	mov r11, rbp[-0x8]
	mov r11[0x8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 2
	mov r11, rbp[-0x8]
	mov r11[0xc], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 3
	mov r11, rbp[-0x8]
	mov r11[0x10], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x10]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 4
	mov r11, rbp[-0x8]
	mov r11[0x14], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x14]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 5
	mov r11, rbp[-0x8]
	mov r11[0x18], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x18]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 6
	mov r11, rbp[-0x8]
	mov r11[0x1c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x1c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 7
	mov r11, rbp[-0x8]
	mov r11[0x20], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x20]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 8
	mov r11, rbp[-0x8]
	mov r11[0x24], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x24]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 9
	mov r11, rbp[-0x8]
	mov r11[0x28], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x28]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 10
	mov r11, rbp[-0x8]
	mov r11[0x2c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x2c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 11
	mov r11, rbp[-0x8]
	mov r11[0x30], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x30]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 12
	mov r11, rbp[-0x8]
	mov r11[0x34], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x34]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 13
	mov r11, rbp[-0x8]
	mov r11[0x38], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x38]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 14
	mov r11, rbp[-0x8]
	mov r11[0x3c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x3c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 15
	mov r11, rbp[-0x8]
	mov r11[0x40], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x40]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 16
	mov r11, rbp[-0x8]
	mov r11[0x44], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x44]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 17
	mov r11, rbp[-0x8]
	mov r11[0x48], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x48]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 18
	mov r11, rbp[-0x8]
	mov r11[0x4c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x4c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 19
	mov r11, rbp[-0x8]
	mov r11[0x50], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x50]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 20
	mov r11, rbp[-0x8]
	mov r11[0x54], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x54]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 21
	mov r11, rbp[-0x8]
	mov r11[0x58], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x58]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 22
	mov r11, rbp[-0x8]
	mov r11[0x5c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x5c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 23
	mov r11, rbp[-0x8]
	mov r11[0x60], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x60]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 24
	mov r11, rbp[-0x8]
	mov r11[0x64], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x64]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 25
	mov r11, rbp[-0x8]
	mov r11[0x68], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x68]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 26
	mov r11, rbp[-0x8]
	mov r11[0x6c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x6c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 27
	mov r11, rbp[-0x8]
	mov r11[0x70], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x70]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 28
	mov r11, rbp[-0x8]
	mov r11[0x74], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x74]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 29
	mov r11, rbp[-0x8]
	mov r11[0x78], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x78]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 30
	mov r11, rbp[-0x8]
	mov r11[0x7c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x7c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 31
	mov r11, rbp[-0x8]
	mov r11[0x80], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x80]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 32
	mov r11, rbp[-0x8]
	mov r11[0x84], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x84]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 33
	mov r11, rbp[-0x8]
	mov r11[0x88], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x88]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 34
	mov r11, rbp[-0x8]
	mov r11[0x8c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x8c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 35
	mov r11, rbp[-0x8]
	mov r11[0x90], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x90]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 36
	mov r11, rbp[-0x8]
	mov r11[0x94], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x94]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 37
	mov r11, rbp[-0x8]
	mov r11[0x98], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x98]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 38
	mov r11, rbp[-0x8]
	mov r11[0x9c], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x9c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 39
	mov r11, rbp[-0x8]
	mov r11[0xa0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xa0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 40
	mov r11, rbp[-0x8]
	mov r11[0xa4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xa4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 41
	mov r11, rbp[-0x8]
	mov r11[0xa8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xa8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 42
	mov r11, rbp[-0x8]
	mov r11[0xac], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xac]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 43
	mov r11, rbp[-0x8]
	mov r11[0xb0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xb0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 44
	mov r11, rbp[-0x8]
	mov r11[0xb4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xb4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 45
	mov r11, rbp[-0x8]
	mov r11[0xb8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xb8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 46
	mov r11, rbp[-0x8]
	mov r11[0xbc], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xbc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 47
	mov r11, rbp[-0x8]
	mov r11[0xc0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xc0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 48
	mov r11, rbp[-0x8]
	mov r11[0xc4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xc4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 49
	mov r11, rbp[-0x8]
	mov r11[0xc8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xc8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 50
	mov r11, rbp[-0x8]
	mov r11[0xcc], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xcc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 51
	mov r11, rbp[-0x8]
	mov r11[0xd0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xd0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 52
	mov r11, rbp[-0x8]
	mov r11[0xd4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xd4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 53
	mov r11, rbp[-0x8]
	mov r11[0xd8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xd8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 54
	mov r11, rbp[-0x8]
	mov r11[0xdc], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xdc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 55
	mov r11, rbp[-0x8]
	mov r11[0xe0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xe0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 56
	mov r11, rbp[-0x8]
	mov r11[0xe4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xe4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 57
	mov r11, rbp[-0x8]
	mov r11[0xe8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xe8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 58
	mov r11, rbp[-0x8]
	mov r11[0xec], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xec]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 59
	mov r11, rbp[-0x8]
	mov r11[0xf0], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xf0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 60
	mov r11, rbp[-0x8]
	mov r11[0xf4], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xf4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 61
	mov r11, rbp[-0x8]
	mov r11[0xf8], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xf8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 62
	mov r11, rbp[-0x8]
	mov r11[0xfc], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0xfc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 63
	mov r11, rbp[-0x8]
	mov r11[0x100], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x100]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 64
	mov r11, rbp[-0x8]
	mov r11[0x104], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x104]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 65
	mov r11, rbp[-0x8]
	mov r11[0x108], eax
	mov rax, rbp[-0x8]
	mov eax, rax[0x108]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
