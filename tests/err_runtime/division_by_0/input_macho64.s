section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/err_runtime/division_by_0/input-d.grug", 0
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

extern grug_call_runtime_error_handler
extern grug_fn_path
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern setjmp
extern game_fn_initialize_bool

global define
define:
	sub rsp, byte 0x8
	call game_fn_define_d wrt ..gotpcrel
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

	; lea rbx, [rel $$]
	; add rbx, _GLOBAL_OFFSET_TABLE_ wrt ..gotpc

	; lea rax, strings[rel 0]
	; mov r11, [rel grug_fn_path wrt ..got]
	; mov [r11], rax

	; lea rax, strings[rel 43]
	; mov r11, [rel grug_fn_name wrt ..got]
	; mov [r11], rax

	; call grug_enable_on_fn_runtime_error_handling wrt ..gotpcrel

	; block
	; xor eax, eax
	; push rax
	; mov eax, 1
	; pop r11
	; cqo
	; idiv r11
	; push rax

	; pop rdi
	; call game_fn_initialize wrt ..gotpcrel
	; unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..gotpcrel

	mov rsp, rbp
	pop rbp
	ret
