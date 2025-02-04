section .data

global define_type
define_type: db "d2", 0

align 8
global globals_size
globals_size: dq 8

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern game_fn_define_d2

global define
define:
	mov eax, __?float32?__(9.0)
    push rax
	mov eax, __?float32?__(8.0)
	movd xmm7, eax
	mov eax, __?float32?__(7.0)
	movd xmm6, eax
	mov eax, __?float32?__(6.0)
	movd xmm5, eax
	mov eax, __?float32?__(5.0)
	movd xmm4, eax
	mov eax, __?float32?__(4.0)
	movd xmm3, eax
	mov eax, __?float32?__(3.0)
	movd xmm2, eax
	mov eax, __?float32?__(2.0)
	movd xmm1, eax
	mov eax, __?float32?__(1.0)
	movd xmm0, eax
	mov eax, 8
    push rax
	mov eax, 7
    push rax
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
	call game_fn_define_d2 wrt ..plt
	add rsp, byte 0x18
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
