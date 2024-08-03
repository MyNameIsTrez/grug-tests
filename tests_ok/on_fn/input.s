section .data

global define_type
define_type: db "h", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

section .text

extern game_fn_define_h

global define
define:
	mov edi, 42
	call game_fn_define_h wrt ..plt
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

	mov rbp[-0xc], esi

	mov rsp, rbp
	pop rbp
	ret
