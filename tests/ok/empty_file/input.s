section .data

align 8
global globals_size
globals_size: dq 8

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
