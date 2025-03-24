section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/or_short_circuit/input-d.grug", 0
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
extern longjmp
extern game_fn_initialize_bool

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

	on_fn_error_handling

	mov eax, 1
	test eax, eax
	je .or_false
	mov eax, 1
	jmp strict .skip
.or_false:
	xor eax, eax
	push rax
	xor eax, eax
	push rax
	mov eax, 1
	pop r11
	check_division_by_0
	check_division_overflow
	cdq
	idiv r11d
	pop r11
	cmp rax, r11
	mov eax, 0
	sete al
	test eax, eax
	mov eax, 0
	setne al

.skip:
	push rax

	pop rdi
	call game_fn_initialize_bool wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 1
	test eax, eax
	je .or_false_fast
	mov eax, 1
	jmp strict .skip_fast
.or_false_fast:
	xor eax, eax
	push rax
	xor eax, eax
	push rax
	mov eax, 1
	pop r11
	cdq
	idiv r11d
	pop r11
	cmp rax, r11
	mov eax, 0
	sete al
	test eax, eax
	mov eax, 0
	setne al

.skip_fast:
	push rax

	pop rdi
	call game_fn_initialize_bool wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
