section .data

global define_type
define_type: db "s", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db "tests_ok/on_fn_overwriting_param/input.grug", 0
	db "on_a", 0

section .text

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_block_mask

extern game_fn_define_s
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_initialize
extern game_fn_sin
extern grug_disable_on_fn_runtime_error_handling
extern _GLOBAL_OFFSET_TABLE_

global define
define:
	call game_fn_define_s wrt ..plt
	ret

global init_globals
init_globals:
	ret

%macro block 0
	xor edx, edx
	mov rsi, rbx[grug_block_mask wrt ..got]
	xor edi, edi
	call sigprocmask wrt ..plt
%endmacro

%macro unblock 0
	push rax
	xor edx, edx
	mov rsi, rbx[grug_block_mask wrt ..got]
	mov edi, 1
	sub rsp, byte 0x8
	call sigprocmask wrt ..plt
	add rsp, byte 0x8
	pop rax
%endmacro

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x20
	mov rbp[-0x8], rbx
	mov rbp[-0x10], rdi
	mov rbp[-0x14], esi
	movss rbp[-0x18], xmm0

	lea rbx, [rel $$]
	add rbx, _GLOBAL_OFFSET_TABLE_ wrt ..gotpc

	lea rax, strings[rel 0]
	mov r11, rbx[grug_on_fn_path wrt ..got]
	mov [r11], rax

	lea rax, strings[rel 44]
	mov r11, rbx[grug_on_fn_name wrt ..got]
	mov [r11], rax

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	; i = 20
	mov eax, 20
	mov rbp[-0x14], eax

	; initialize(i)
	block
	mov eax, rbp[-0x14]
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt
	unblock

	; f = 30.0
	mov eax, __?float32?__(30.0)
	mov rbp[-0x18], eax

	block
	; sin(f)
	mov eax, rbp[-0x18]
	push rax
	pop rax
	movd xmm0, eax
	call game_fn_sin wrt ..plt
	movd eax, xmm0
	unblock

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]
	mov rsp, rbp
	pop rbp
	ret
