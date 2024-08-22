section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

section .text

extern grug_block_mask

extern game_fn_define_d
extern grug_enable_on_fn_runtime_error_handling
extern sigprocmask
extern game_fn_nothing
extern grug_disable_on_fn_runtime_error_handling
extern _GLOBAL_OFFSET_TABLE_

global define
define:
	call game_fn_define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x18
	mov rbp[-0x8], rdi

	; From https://www.nasm.us/xdoc/2.15/html/nasmdo10.html
	; We want rbx since it's a callee-saved register,
	; meaning game fn calls can't overwrite it, meaning we don't have to
	; reload the global offset table every time we want to access a global
	push rbx
	lea rbx, [rel $$]
	add rbx, _GLOBAL_OFFSET_TABLE_ wrt ..gotpc

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	xor edi, edi
	mov rsi, [rbx + grug_block_mask wrt ..got]
	xor edx, edx
	call sigprocmask wrt ..plt

	call game_fn_nothing wrt ..plt

	mov edi, 1
	mov rsi, [rbx + grug_block_mask wrt ..got]
	xor edx, edx
	call sigprocmask wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8]

	mov rsp, rbp
	pop rbp
	ret
