global define_type
global globals_size

global define
global init_globals

section .data

define_type: db "n", 0

globals_size: dq 0

section .text

extern define_n

define:
	mov rdi, 1
	mov rsi, 2
	mov rdx, 3
	mov rcx, 4
	mov r8, 5
	mov r9, 6
	call define_n wrt ..plt
	ret

init_globals:
	ret
