section .data

global define_type
define_type: db "a", 0

align 8
global globals_size
globals_size: dq 256

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
	mov dword rdi[0x80], 33
	mov dword rdi[0x84], 34
	mov dword rdi[0x88], 35
	mov dword rdi[0x8c], 36
	mov dword rdi[0x90], 37
	mov dword rdi[0x94], 38
	mov dword rdi[0x98], 39
	mov dword rdi[0x9c], 40
	mov dword rdi[0xa0], 41
	mov dword rdi[0xa4], 42
	mov dword rdi[0xa8], 43
	mov dword rdi[0xac], 44
	mov dword rdi[0xb0], 45
	mov dword rdi[0xb4], 46
	mov dword rdi[0xb8], 47
	mov dword rdi[0xbc], 48
	mov dword rdi[0xc0], 49
	mov dword rdi[0xc4], 50
	mov dword rdi[0xc8], 51
	mov dword rdi[0xcc], 52
	mov dword rdi[0xd0], 53
	mov dword rdi[0xd4], 54
	mov dword rdi[0xd8], 55
	mov dword rdi[0xdc], 56
	mov dword rdi[0xe0], 57
	mov dword rdi[0xe4], 58
	mov dword rdi[0xe8], 59
	mov dword rdi[0xec], 60
	mov dword rdi[0xf0], 61
	mov dword rdi[0xf4], 62
	mov dword rdi[0xf8], 63
	mov dword rdi[0xfc], 64
	ret
