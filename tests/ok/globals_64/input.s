section .data

global define_type
define_type: db "a", 0

align 8
global globals_size
globals_size: dq 256

global strings
strings:

align 8
global dll_resources_size
dll_resources_size: dq 0

section .text

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_block_mask

extern game_fn_define_a

global define
define:
	call game_fn_define_a wrt ..plt
	ret

global init_globals
init_globals:
	mov eax, 1
	mov rdi[byte 0x0], eax
	mov eax, 2
	mov rdi[0x4], eax
	mov eax, 3
	mov rdi[0x8], eax
	mov eax, 4
	mov rdi[0xc], eax
	mov eax, 5
	mov rdi[0x10], eax
	mov eax, 6
	mov rdi[0x14], eax
	mov eax, 7
	mov rdi[0x18], eax
	mov eax, 8
	mov rdi[0x1c], eax
	mov eax, 9
	mov rdi[0x20], eax
	mov eax, 10
	mov rdi[0x24], eax
	mov eax, 11
	mov rdi[0x28], eax
	mov eax, 12
	mov rdi[0x2c], eax
	mov eax, 13
	mov rdi[0x30], eax
	mov eax, 14
	mov rdi[0x34], eax
	mov eax, 15
	mov rdi[0x38], eax
	mov eax, 16
	mov rdi[0x3c], eax
	mov eax, 17
	mov rdi[0x40], eax
	mov eax, 18
	mov rdi[0x44], eax
	mov eax, 19
	mov rdi[0x48], eax
	mov eax, 20
	mov rdi[0x4c], eax
	mov eax, 21
	mov rdi[0x50], eax
	mov eax, 22
	mov rdi[0x54], eax
	mov eax, 23
	mov rdi[0x58], eax
	mov eax, 24
	mov rdi[0x5c], eax
	mov eax, 25
	mov rdi[0x60], eax
	mov eax, 26
	mov rdi[0x64], eax
	mov eax, 27
	mov rdi[0x68], eax
	mov eax, 28
	mov rdi[0x6c], eax
	mov eax, 29
	mov rdi[0x70], eax
	mov eax, 30
	mov rdi[0x74], eax
	mov eax, 31
	mov rdi[0x78], eax
	mov eax, 32
	mov rdi[0x7c], eax
	mov eax, 33
	mov rdi[0x80], eax
	mov eax, 34
	mov rdi[0x84], eax
	mov eax, 35
	mov rdi[0x88], eax
	mov eax, 36
	mov rdi[0x8c], eax
	mov eax, 37
	mov rdi[0x90], eax
	mov eax, 38
	mov rdi[0x94], eax
	mov eax, 39
	mov rdi[0x98], eax
	mov eax, 40
	mov rdi[0x9c], eax
	mov eax, 41
	mov rdi[0xa0], eax
	mov eax, 42
	mov rdi[0xa4], eax
	mov eax, 43
	mov rdi[0xa8], eax
	mov eax, 44
	mov rdi[0xac], eax
	mov eax, 45
	mov rdi[0xb0], eax
	mov eax, 46
	mov rdi[0xb4], eax
	mov eax, 47
	mov rdi[0xb8], eax
	mov eax, 48
	mov rdi[0xbc], eax
	mov eax, 49
	mov rdi[0xc0], eax
	mov eax, 50
	mov rdi[0xc4], eax
	mov eax, 51
	mov rdi[0xc8], eax
	mov eax, 52
	mov rdi[0xcc], eax
	mov eax, 53
	mov rdi[0xd0], eax
	mov eax, 54
	mov rdi[0xd4], eax
	mov eax, 55
	mov rdi[0xd8], eax
	mov eax, 56
	mov rdi[0xdc], eax
	mov eax, 57
	mov rdi[0xe0], eax
	mov eax, 58
	mov rdi[0xe4], eax
	mov eax, 59
	mov rdi[0xe8], eax
	mov eax, 60
	mov rdi[0xec], eax
	mov eax, 61
	mov rdi[0xf0], eax
	mov eax, 62
	mov rdi[0xf4], eax
	mov eax, 63
	mov rdi[0xf8], eax
	mov eax, 64
	mov rdi[0xfc], eax
	ret
