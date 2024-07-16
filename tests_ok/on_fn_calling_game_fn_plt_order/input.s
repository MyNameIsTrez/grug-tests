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
extern magic
extern initialize
extern identity
extern max

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
	call magic wrt ..plt
	mov rdi, 42
	call initialize wrt ..plt
	mov rdi, 69
	call identity wrt ..plt
	mov rdi, 1337
	mov rsi, 8192
	call max wrt ..plt
	ret
