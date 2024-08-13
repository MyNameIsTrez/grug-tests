section .data

global define_type
define_type: db "a", 0

align 8
global globals_size
globals_size: dq 128

section .text

extern game_fn_define_a

global define
define:
	call game_fn_define_a wrt ..plt
	ret

global init_globals
init_globals:
	mov eax, 1
	mov rdi[byte 0x0], rax
	mov eax, 2
	mov rdi[0x4], rax
	mov eax, 3
	mov rdi[0x8], rax
	mov eax, 4
	mov rdi[0xc], rax
	mov eax, 5
	mov rdi[0x10], rax
	mov eax, 6
	mov rdi[0x14], rax
	mov eax, 7
	mov rdi[0x18], rax
	mov eax, 8
	mov rdi[0x1c], rax
	mov eax, 9
	mov rdi[0x20], rax
	mov eax, 10
	mov rdi[0x24], rax
	mov eax, 11
	mov rdi[0x28], rax
	mov eax, 12
	mov rdi[0x2c], rax
	mov eax, 13
	mov rdi[0x30], rax
	mov eax, 14
	mov rdi[0x34], rax
	mov eax, 15
	mov rdi[0x38], rax
	mov eax, 16
	mov rdi[0x3c], rax
	mov eax, 17
	mov rdi[0x40], rax
	mov eax, 18
	mov rdi[0x44], rax
	mov eax, 19
	mov rdi[0x48], rax
	mov eax, 20
	mov rdi[0x4c], rax
	mov eax, 21
	mov rdi[0x50], rax
	mov eax, 22
	mov rdi[0x54], rax
	mov eax, 23
	mov rdi[0x58], rax
	mov eax, 24
	mov rdi[0x5c], rax
	mov eax, 25
	mov rdi[0x60], rax
	mov eax, 26
	mov rdi[0x64], rax
	mov eax, 27
	mov rdi[0x68], rax
	mov eax, 28
	mov rdi[0x6c], rax
	mov eax, 29
	mov rdi[0x70], rax
	mov eax, 30
	mov rdi[0x74], rax
	mov eax, 31
	mov rdi[0x78], rax
	mov eax, 32
	mov rdi[0x7c], rax
	ret
