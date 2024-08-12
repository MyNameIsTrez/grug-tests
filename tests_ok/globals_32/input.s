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
	mov dword rdi[byte 0x0], 1
	mov dword rdi[0x4], 2
	mov dword rdi[0x8], 3
	mov dword rdi[0xc], 4
	mov dword rdi[0x10], 5
	mov dword rdi[0x14], 6
	mov dword rdi[0x18], 7
	mov dword rdi[0x1c], 8
	mov dword rdi[0x20], 9
	mov dword rdi[0x24], 10
	mov dword rdi[0x28], 11
	mov dword rdi[0x2c], 12
	mov dword rdi[0x30], 13
	mov dword rdi[0x34], 14
	mov dword rdi[0x38], 15
	mov dword rdi[0x3c], 16
	mov dword rdi[0x40], 17
	mov dword rdi[0x44], 18
	mov dword rdi[0x48], 19
	mov dword rdi[0x4c], 20
	mov dword rdi[0x50], 21
	mov dword rdi[0x54], 22
	mov dword rdi[0x58], 23
	mov dword rdi[0x5c], 24
	mov dword rdi[0x60], 25
	mov dword rdi[0x64], 26
	mov dword rdi[0x68], 27
	mov dword rdi[0x6c], 28
	mov dword rdi[0x70], 29
	mov dword rdi[0x74], 30
	mov dword rdi[0x78], 31
	mov dword rdi[0x7c], 32
	ret
