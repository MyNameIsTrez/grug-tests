section .data

align 8
global globals_size
globals_size: dq 12

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/on_fn_calling_no_game_fn_but_with_global/input-d.grug", 0
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

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode

global init_globals
init_globals:
	mov rdi[0x0], rsi
	mov eax, 0x2a
	mov rdi[0x8], eax
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

	mov eax, 1
	test eax, eax
	je strict .false_safe
.false_safe:

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 1
	test eax, eax
	je strict .false_fast
.false_fast:

	mov rsp, rbp
	pop rbp
	ret
