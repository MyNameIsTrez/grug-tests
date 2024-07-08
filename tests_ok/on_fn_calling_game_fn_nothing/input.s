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

extern define_h
extern nothing

global define
define:
	mov rdi, 42
	call define_h wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
	call nothing wrt ..plt
	ret
