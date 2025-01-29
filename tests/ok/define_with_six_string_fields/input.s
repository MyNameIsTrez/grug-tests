section .data

global define_type
define_type: db "o", 0

align 8
global globals_size
globals_size: dq 8

z:
	db "z", 0
y:
	db "y", 0
x:
	db "x", 0
w:
	db "w", 0
v:
	db "v", 0
u:
	db "u", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern game_fn_define_o

global define
define:
	sub rsp, byte 0x8
	lea rax, [rel z]
	mov r9, rax
	lea rax, [rel y]
	mov r8, rax
	lea rax, [rel x]
	mov rcx, rax
	lea rax, [rel w]
	mov rdx, rax
	lea rax, [rel v]
	mov rsi, rax
	lea rax, [rel u]
	mov rdi, rax
	call game_fn_define_o wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
