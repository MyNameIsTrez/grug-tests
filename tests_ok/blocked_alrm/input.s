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

extern game_fn_define_d
extern grug_enable_on_fn_runtime_error_handling
extern game_fn_nothing
extern grug_disable_on_fn_runtime_error_handling
extern sigprocmask
extern grug_block_alrm_mask
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
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	; From https://www.nasm.us/xdoc/2.15/html/nasmdo10.html
	push rbx
	lea rbx, [rel .get_GOT]
.get_GOT:
	add rbx, _GLOBAL_OFFSET_TABLE_ + $$ - .get_GOT wrt ..gotpc

	call grug_enable_on_fn_runtime_error_handling wrt ..plt

	xor edi, edi
	mov rsi, [rbx + grug_block_alrm_mask wrt ..got]
	xor edx, edx
	call sigprocmask wrt ..plt

	call game_fn_nothing wrt ..plt

	mov edi, 1
	mov rsi, [rbx + grug_block_alrm_mask wrt ..got]
	xor edx, edx
	call sigprocmask wrt ..plt

	call grug_disable_on_fn_runtime_error_handling wrt ..plt

	mov rbx, rbp[-0x8] ; TODO: It is -0x4 in https://www.nasm.us/xdoc/2.15/html/nasmdo10.html

	mov rsp, rbp
	pop rbp
	ret
