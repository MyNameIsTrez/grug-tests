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
	db "tests/ok/helper_fn_overwriting_param/input.grug", 0
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
extern grug_max_rsp
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
extern longjmp
extern game_fn_initialize
extern game_fn_sin

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

	set_max_rsp

	set_time_limit

	error_handling

	; helper_foo_safe(2, 3.0)
	mov eax, __?float32?__(3.0)
	push rax
	mov eax, 2
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	pop rax
	movd xmm0, eax
	call helper_foo_safe

	mov rsp, rbp
	pop rbp
	ret

.fast:
	; helper_foo_fast(2, 3.0)
	mov eax, __?float32?__(3.0)
	push rax
	mov eax, 2
	push rax
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	pop rsi
	pop rax
	movd xmm0, eax
	call helper_foo_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_safe
helper_foo_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	mov rbp[-0xc], esi
	movss rbp[-0x10], xmm0
	check_stack_overflow
	check_time_limit_exceeded

	; i = 20
	mov eax, 20
	mov rbp[-0xc], eax

	; initialize(i)
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	; f = 30.0
	mov eax, __?float32?__(30.0)
	mov rbp[-0x10], eax

	; sin(f)
	mov eax, rbp[-0x10]
	push rax
	pop rax
	movd xmm0, eax
	call game_fn_sin wrt ..plt
	movd eax, xmm0

	mov rsp, rbp
	pop rbp
	ret

global helper_foo_fast
helper_foo_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	mov rbp[-0xc], esi
	movss rbp[-0x10], xmm0

	; i = 20
	mov eax, 20
	mov rbp[-0xc], eax

	; initialize(i)
	mov eax, rbp[-0xc]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	; f = 30.0
	mov eax, __?float32?__(30.0)
	mov rbp[-0x10], eax

	; sin(f)
	mov eax, rbp[-0x10]
	push rax
	pop rax
	movd xmm0, eax
	call game_fn_sin wrt ..plt
	movd eax, xmm0

	mov rsp, rbp
	pop rbp
	ret
