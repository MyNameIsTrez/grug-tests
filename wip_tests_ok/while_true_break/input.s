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

extern define_d
extern nothing

global define
define:
	call define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
	call nothing wrt ..plt

	mov eax, 1
	test rax, rax
	je strict $+0x1a
	call nothing wrt ..plt
	jmp strict $+0xf
	call nothing wrt ..plt
	jmp strict $-0x1d

	call nothing wrt ..plt

	ret
