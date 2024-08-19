section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

global strings
strings:
	db "foo", 0

section .text

extern game_fn_define_d
extern alarm
extern game_fn_say

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
	mov edi, 1
	call alarm wrt ..plt

	mov rax, rbp[-0x8]
	push rax

	lea rax, strings[rel 0]
	push rax

	pop rsi
	pop rdi
	call helper_shout

	xor edi, edi
	call alarm wrt ..plt
	mov rsp, rbp
	pop rbp
	ret

global helper_shout
helper_shout:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rbp[-0x10], rsi
	mov rax, rbp[-0x10]
	push rax

	pop rdi
	call game_fn_say wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
