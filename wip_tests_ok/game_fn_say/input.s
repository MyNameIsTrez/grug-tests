section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "foo", 0

global on_fns
on_fns:
	dq on_a

section .text

extern define_d
extern say

global define
define:
	call define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
	lea rax, strings[rel 0]
	push rax
	pop rdi
	call say wrt ..plt
	ret
