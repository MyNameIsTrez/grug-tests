section .data

global define_type
define_type: db "p", 0

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

extern define_p

global define
define:
	lea rdi, [rel strings+0]
	call define_p wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
    push rbp
    mov rbp, rsp
	sub rsp, 0x8
    mov rbp[-0x8], rdi

    mov rsp, rbp
    pop rbp
	ret
