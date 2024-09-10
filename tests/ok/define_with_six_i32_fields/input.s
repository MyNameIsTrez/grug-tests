section .data

global define_type
define_type: db "n", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:

align 8
global dll_resources_size
dll_resources_size: dq 0

section .text

extern game_fn_define_n

global define
define:
	mov eax, 6
	mov r9, rax
	mov eax, 5
	mov r8, rax
	mov eax, 4
	mov rcx, rax
	mov eax, 3
	mov rdx, rax
	mov eax, 2
	mov rsi, rax
	mov eax, 1
	mov rdi, rax
	call game_fn_define_n wrt ..plt
	ret

global init_globals
init_globals:
	ret
