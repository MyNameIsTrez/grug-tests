section .data

global define_type
define_type: db "i", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq 0
	dq on_b

section .text

extern define_i

global define
define:
	mov rdi, 42
	mov rsi, 69
	call define_i wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_b
on_b:
	ret
