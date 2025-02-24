section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/continue/input.grug", 0
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
extern grug_max_time
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern grug_current_time
extern game_fn_define_d
extern clock_gettime
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_nothing
extern longjmp

global define
define:
	sub rsp, byte 0x8
	call game_fn_define_d wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
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

	set_time_limit

	error_handling

	xor eax, eax
	mov rbp[-0xc], eax

.repeat:
	mov eax, 2
	push rax

	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11

	mov eax, 0x0
	setl al

	test eax, eax
	je strict .skip

	call game_fn_nothing wrt ..plt

	; i++
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	add eax, r11d
	check_overflow
	mov rbp[-0xc], eax

	check_time_limit_exceeded
	jmp strict .repeat

	call game_fn_nothing wrt ..plt

	check_time_limit_exceeded
	jmp strict .repeat
.skip:

	mov rsp, rbp
	pop rbp
	ret

.fast:
	xor eax, eax
	mov rbp[-0xc], eax

.repeat_fast:
	mov eax, 2
	push rax

	mov eax, rbp[-0xc]
	pop r11
	cmp rax, r11

	mov eax, 0x0
	setl al

	test eax, eax
	je strict .skip_fast

	call game_fn_nothing wrt ..plt

	; i++
	mov eax, 1
	push rax
	mov eax, rbp[-0xc]
	pop r11
	add eax, r11d
	mov rbp[-0xc], eax

	jmp strict .repeat_fast

	call game_fn_nothing wrt ..plt

	jmp strict .repeat_fast
.skip_fast:

	mov rsp, rbp
	pop rbp
	ret
