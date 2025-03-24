section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/sub_rsp_32_bits_local_variables_i32/input-d.grug", 0
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
	mov rdi[0x0], rsi
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, 0x110
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_on_fn_name_and_path

	mov eax, 1
	mov rbp[-0xc], eax
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 2
	mov rbp[-0x10], eax
	mov eax, rbp[-0x10]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 3
	mov rbp[-0x14], eax
	mov eax, rbp[-0x14]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 4
	mov rbp[-0x18], eax
	mov eax, rbp[-0x18]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 5
	mov rbp[-0x1c], eax
	mov eax, rbp[-0x1c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 6
	mov rbp[-0x20], eax
	mov eax, rbp[-0x20]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 7
	mov rbp[-0x24], eax
	mov eax, rbp[-0x24]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 8
	mov rbp[-0x28], eax
	mov eax, rbp[-0x28]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 9
	mov rbp[-0x2c], eax
	mov eax, rbp[-0x2c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 10
	mov rbp[-0x30], eax
	mov eax, rbp[-0x30]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 11
	mov rbp[-0x34], eax
	mov eax, rbp[-0x34]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 12
	mov rbp[-0x38], eax
	mov eax, rbp[-0x38]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 13
	mov rbp[-0x3c], eax
	mov eax, rbp[-0x3c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 14
	mov rbp[-0x40], eax
	mov eax, rbp[-0x40]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 15
	mov rbp[-0x44], eax
	mov eax, rbp[-0x44]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 16
	mov rbp[-0x48], eax
	mov eax, rbp[-0x48]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 17
	mov rbp[-0x4c], eax
	mov eax, rbp[-0x4c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 18
	mov rbp[-0x50], eax
	mov eax, rbp[-0x50]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 19
	mov rbp[-0x54], eax
	mov eax, rbp[-0x54]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 20
	mov rbp[-0x58], eax
	mov eax, rbp[-0x58]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 21
	mov rbp[-0x5c], eax
	mov eax, rbp[-0x5c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 22
	mov rbp[-0x60], eax
	mov eax, rbp[-0x60]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 23
	mov rbp[-0x64], eax
	mov eax, rbp[-0x64]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 24
	mov rbp[-0x68], eax
	mov eax, rbp[-0x68]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 25
	mov rbp[-0x6c], eax
	mov eax, rbp[-0x6c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 26
	mov rbp[-0x70], eax
	mov eax, rbp[-0x70]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 27
	mov rbp[-0x74], eax
	mov eax, rbp[-0x74]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 28
	mov rbp[-0x78], eax
	mov eax, rbp[-0x78]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 29
	mov rbp[-0x7c], eax
	mov eax, rbp[-0x7c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 30
	mov rbp[-0x80], eax
	mov eax, rbp[-0x80]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 31
	mov rbp[-0x84], eax
	mov eax, rbp[-0x84]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 32
	mov rbp[-0x88], eax
	mov eax, rbp[-0x88]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 33
	mov rbp[-0x8c], eax
	mov eax, rbp[-0x8c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 34
	mov rbp[-0x90], eax
	mov eax, rbp[-0x90]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 35
	mov rbp[-0x94], eax
	mov eax, rbp[-0x94]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 36
	mov rbp[-0x98], eax
	mov eax, rbp[-0x98]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 37
	mov rbp[-0x9c], eax
	mov eax, rbp[-0x9c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 38
	mov rbp[-0xa0], eax
	mov eax, rbp[-0xa0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 39
	mov rbp[-0xa4], eax
	mov eax, rbp[-0xa4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 40
	mov rbp[-0xa8], eax
	mov eax, rbp[-0xa8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 41
	mov rbp[-0xac], eax
	mov eax, rbp[-0xac]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 42
	mov rbp[-0xb0], eax
	mov eax, rbp[-0xb0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 43
	mov rbp[-0xb4], eax
	mov eax, rbp[-0xb4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 44
	mov rbp[-0xb8], eax
	mov eax, rbp[-0xb8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 45
	mov rbp[-0xbc], eax
	mov eax, rbp[-0xbc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 46
	mov rbp[-0xc0], eax
	mov eax, rbp[-0xc0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 47
	mov rbp[-0xc4], eax
	mov eax, rbp[-0xc4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 48
	mov rbp[-0xc8], eax
	mov eax, rbp[-0xc8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 49
	mov rbp[-0xcc], eax
	mov eax, rbp[-0xcc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 50
	mov rbp[-0xd0], eax
	mov eax, rbp[-0xd0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 51
	mov rbp[-0xd4], eax
	mov eax, rbp[-0xd4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 52
	mov rbp[-0xd8], eax
	mov eax, rbp[-0xd8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 53
	mov rbp[-0xdc], eax
	mov eax, rbp[-0xdc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 54
	mov rbp[-0xe0], eax
	mov eax, rbp[-0xe0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 55
	mov rbp[-0xe4], eax
	mov eax, rbp[-0xe4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 56
	mov rbp[-0xe8], eax
	mov eax, rbp[-0xe8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 57
	mov rbp[-0xec], eax
	mov eax, rbp[-0xec]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 58
	mov rbp[-0xf0], eax
	mov eax, rbp[-0xf0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 59
	mov rbp[-0xf4], eax
	mov eax, rbp[-0xf4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 60
	mov rbp[-0xf8], eax
	mov eax, rbp[-0xf8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 61
	mov rbp[-0xfc], eax
	mov eax, rbp[-0xfc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 62
	mov rbp[-0x100], eax
	mov eax, rbp[-0x100]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 63
	mov rbp[-0x104], eax
	mov eax, rbp[-0x104]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 64
	mov rbp[-0x108], eax
	mov eax, rbp[-0x108]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 65
	mov rbp[-0x10c], eax
	mov eax, rbp[-0x10c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 1
	mov rbp[-0xc], eax
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 2
	mov rbp[-0x10], eax
	mov eax, rbp[-0x10]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 3
	mov rbp[-0x14], eax
	mov eax, rbp[-0x14]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 4
	mov rbp[-0x18], eax
	mov eax, rbp[-0x18]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 5
	mov rbp[-0x1c], eax
	mov eax, rbp[-0x1c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 6
	mov rbp[-0x20], eax
	mov eax, rbp[-0x20]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 7
	mov rbp[-0x24], eax
	mov eax, rbp[-0x24]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 8
	mov rbp[-0x28], eax
	mov eax, rbp[-0x28]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 9
	mov rbp[-0x2c], eax
	mov eax, rbp[-0x2c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 10
	mov rbp[-0x30], eax
	mov eax, rbp[-0x30]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 11
	mov rbp[-0x34], eax
	mov eax, rbp[-0x34]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 12
	mov rbp[-0x38], eax
	mov eax, rbp[-0x38]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 13
	mov rbp[-0x3c], eax
	mov eax, rbp[-0x3c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 14
	mov rbp[-0x40], eax
	mov eax, rbp[-0x40]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 15
	mov rbp[-0x44], eax
	mov eax, rbp[-0x44]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 16
	mov rbp[-0x48], eax
	mov eax, rbp[-0x48]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 17
	mov rbp[-0x4c], eax
	mov eax, rbp[-0x4c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 18
	mov rbp[-0x50], eax
	mov eax, rbp[-0x50]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 19
	mov rbp[-0x54], eax
	mov eax, rbp[-0x54]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 20
	mov rbp[-0x58], eax
	mov eax, rbp[-0x58]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 21
	mov rbp[-0x5c], eax
	mov eax, rbp[-0x5c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 22
	mov rbp[-0x60], eax
	mov eax, rbp[-0x60]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 23
	mov rbp[-0x64], eax
	mov eax, rbp[-0x64]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 24
	mov rbp[-0x68], eax
	mov eax, rbp[-0x68]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 25
	mov rbp[-0x6c], eax
	mov eax, rbp[-0x6c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 26
	mov rbp[-0x70], eax
	mov eax, rbp[-0x70]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 27
	mov rbp[-0x74], eax
	mov eax, rbp[-0x74]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 28
	mov rbp[-0x78], eax
	mov eax, rbp[-0x78]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 29
	mov rbp[-0x7c], eax
	mov eax, rbp[-0x7c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 30
	mov rbp[-0x80], eax
	mov eax, rbp[-0x80]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 31
	mov rbp[-0x84], eax
	mov eax, rbp[-0x84]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 32
	mov rbp[-0x88], eax
	mov eax, rbp[-0x88]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 33
	mov rbp[-0x8c], eax
	mov eax, rbp[-0x8c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 34
	mov rbp[-0x90], eax
	mov eax, rbp[-0x90]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 35
	mov rbp[-0x94], eax
	mov eax, rbp[-0x94]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 36
	mov rbp[-0x98], eax
	mov eax, rbp[-0x98]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 37
	mov rbp[-0x9c], eax
	mov eax, rbp[-0x9c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 38
	mov rbp[-0xa0], eax
	mov eax, rbp[-0xa0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 39
	mov rbp[-0xa4], eax
	mov eax, rbp[-0xa4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 40
	mov rbp[-0xa8], eax
	mov eax, rbp[-0xa8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 41
	mov rbp[-0xac], eax
	mov eax, rbp[-0xac]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 42
	mov rbp[-0xb0], eax
	mov eax, rbp[-0xb0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 43
	mov rbp[-0xb4], eax
	mov eax, rbp[-0xb4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 44
	mov rbp[-0xb8], eax
	mov eax, rbp[-0xb8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 45
	mov rbp[-0xbc], eax
	mov eax, rbp[-0xbc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 46
	mov rbp[-0xc0], eax
	mov eax, rbp[-0xc0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 47
	mov rbp[-0xc4], eax
	mov eax, rbp[-0xc4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 48
	mov rbp[-0xc8], eax
	mov eax, rbp[-0xc8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 49
	mov rbp[-0xcc], eax
	mov eax, rbp[-0xcc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 50
	mov rbp[-0xd0], eax
	mov eax, rbp[-0xd0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 51
	mov rbp[-0xd4], eax
	mov eax, rbp[-0xd4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 52
	mov rbp[-0xd8], eax
	mov eax, rbp[-0xd8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 53
	mov rbp[-0xdc], eax
	mov eax, rbp[-0xdc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 54
	mov rbp[-0xe0], eax
	mov eax, rbp[-0xe0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 55
	mov rbp[-0xe4], eax
	mov eax, rbp[-0xe4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 56
	mov rbp[-0xe8], eax
	mov eax, rbp[-0xe8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 57
	mov rbp[-0xec], eax
	mov eax, rbp[-0xec]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 58
	mov rbp[-0xf0], eax
	mov eax, rbp[-0xf0]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 59
	mov rbp[-0xf4], eax
	mov eax, rbp[-0xf4]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 60
	mov rbp[-0xf8], eax
	mov eax, rbp[-0xf8]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 61
	mov rbp[-0xfc], eax
	mov eax, rbp[-0xfc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 62
	mov rbp[-0x100], eax
	mov eax, rbp[-0x100]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 63
	mov rbp[-0x104], eax
	mov eax, rbp[-0x104]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 64
	mov rbp[-0x108], eax
	mov eax, rbp[-0x108]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	mov eax, 65
	mov rbp[-0x10c], eax
	mov eax, rbp[-0x10c]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
