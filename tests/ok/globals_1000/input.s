section .data

align 8
global globals_size
globals_size: dq 4008

init_globals_fn_path:
	db "tests/ok/globals_1000/input-a.grug", 0
init_globals_fn_name:
	db "init_globals", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern grug_runtime_error_handler
extern grug_fn_path
extern grug_runtime_error_jmp_buffer
extern grug_fn_name
extern grug_has_runtime_error_happened
extern grug_on_fns_in_safe_mode
extern setjmp
extern grug_get_runtime_error_reason

global init_globals
init_globals:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rdi[0x0], rsi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_init_globals_fn_name_and_path

	init_globals_fn_error_handling

	mov eax, 1
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], eax

	mov eax, 2
	mov r11, rbp[-0x8]
	mov r11[byte 0xc], eax

	mov eax, 3
	mov r11, rbp[-0x8]
	mov r11[byte 0x10], eax

	mov eax, 4
	mov r11, rbp[-0x8]
	mov r11[byte 0x14], eax

	mov eax, 5
	mov r11, rbp[-0x8]
	mov r11[byte 0x18], eax

	mov eax, 6
	mov r11, rbp[-0x8]
	mov r11[byte 0x1c], eax

	mov eax, 7
	mov r11, rbp[-0x8]
	mov r11[byte 0x20], eax

	mov eax, 8
	mov r11, rbp[-0x8]
	mov r11[byte 0x24], eax

	mov eax, 9
	mov r11, rbp[-0x8]
	mov r11[byte 0x28], eax

	mov eax, 10
	mov r11, rbp[-0x8]
	mov r11[byte 0x2c], eax

	mov eax, 11
	mov r11, rbp[-0x8]
	mov r11[byte 0x30], eax

	mov eax, 12
	mov r11, rbp[-0x8]
	mov r11[byte 0x34], eax

	mov eax, 13
	mov r11, rbp[-0x8]
	mov r11[byte 0x38], eax

	mov eax, 14
	mov r11, rbp[-0x8]
	mov r11[byte 0x3c], eax

	mov eax, 15
	mov r11, rbp[-0x8]
	mov r11[byte 0x40], eax

	mov eax, 16
	mov r11, rbp[-0x8]
	mov r11[byte 0x44], eax

	mov eax, 17
	mov r11, rbp[-0x8]
	mov r11[byte 0x48], eax

	mov eax, 18
	mov r11, rbp[-0x8]
	mov r11[byte 0x4c], eax

	mov eax, 19
	mov r11, rbp[-0x8]
	mov r11[byte 0x50], eax

	mov eax, 20
	mov r11, rbp[-0x8]
	mov r11[byte 0x54], eax

	mov eax, 21
	mov r11, rbp[-0x8]
	mov r11[byte 0x58], eax

	mov eax, 22
	mov r11, rbp[-0x8]
	mov r11[byte 0x5c], eax

	mov eax, 23
	mov r11, rbp[-0x8]
	mov r11[byte 0x60], eax

	mov eax, 24
	mov r11, rbp[-0x8]
	mov r11[byte 0x64], eax

	mov eax, 25
	mov r11, rbp[-0x8]
	mov r11[byte 0x68], eax

	mov eax, 26
	mov r11, rbp[-0x8]
	mov r11[byte 0x6c], eax

	mov eax, 27
	mov r11, rbp[-0x8]
	mov r11[byte 0x70], eax

	mov eax, 28
	mov r11, rbp[-0x8]
	mov r11[byte 0x74], eax

	mov eax, 29
	mov r11, rbp[-0x8]
	mov r11[byte 0x78], eax

	mov eax, 30
	mov r11, rbp[-0x8]
	mov r11[byte 0x7c], eax

	mov eax, 31
	mov r11, rbp[-0x8]
	mov r11[0x80], eax

	mov eax, 32
	mov r11, rbp[-0x8]
	mov r11[0x84], eax

	mov eax, 33
	mov r11, rbp[-0x8]
	mov r11[0x88], eax

	mov eax, 34
	mov r11, rbp[-0x8]
	mov r11[0x8c], eax

	mov eax, 35
	mov r11, rbp[-0x8]
	mov r11[0x90], eax

	mov eax, 36
	mov r11, rbp[-0x8]
	mov r11[0x94], eax

	mov eax, 37
	mov r11, rbp[-0x8]
	mov r11[0x98], eax

	mov eax, 38
	mov r11, rbp[-0x8]
	mov r11[0x9c], eax

	mov eax, 39
	mov r11, rbp[-0x8]
	mov r11[0xa0], eax

	mov eax, 40
	mov r11, rbp[-0x8]
	mov r11[0xa4], eax

	mov eax, 41
	mov r11, rbp[-0x8]
	mov r11[0xa8], eax

	mov eax, 42
	mov r11, rbp[-0x8]
	mov r11[0xac], eax

	mov eax, 43
	mov r11, rbp[-0x8]
	mov r11[0xb0], eax

	mov eax, 44
	mov r11, rbp[-0x8]
	mov r11[0xb4], eax

	mov eax, 45
	mov r11, rbp[-0x8]
	mov r11[0xb8], eax

	mov eax, 46
	mov r11, rbp[-0x8]
	mov r11[0xbc], eax

	mov eax, 47
	mov r11, rbp[-0x8]
	mov r11[0xc0], eax

	mov eax, 48
	mov r11, rbp[-0x8]
	mov r11[0xc4], eax

	mov eax, 49
	mov r11, rbp[-0x8]
	mov r11[0xc8], eax

	mov eax, 50
	mov r11, rbp[-0x8]
	mov r11[0xcc], eax

	mov eax, 51
	mov r11, rbp[-0x8]
	mov r11[0xd0], eax

	mov eax, 52
	mov r11, rbp[-0x8]
	mov r11[0xd4], eax

	mov eax, 53
	mov r11, rbp[-0x8]
	mov r11[0xd8], eax

	mov eax, 54
	mov r11, rbp[-0x8]
	mov r11[0xdc], eax

	mov eax, 55
	mov r11, rbp[-0x8]
	mov r11[0xe0], eax

	mov eax, 56
	mov r11, rbp[-0x8]
	mov r11[0xe4], eax

	mov eax, 57
	mov r11, rbp[-0x8]
	mov r11[0xe8], eax

	mov eax, 58
	mov r11, rbp[-0x8]
	mov r11[0xec], eax

	mov eax, 59
	mov r11, rbp[-0x8]
	mov r11[0xf0], eax

	mov eax, 60
	mov r11, rbp[-0x8]
	mov r11[0xf4], eax

	mov eax, 61
	mov r11, rbp[-0x8]
	mov r11[0xf8], eax

	mov eax, 62
	mov r11, rbp[-0x8]
	mov r11[0xfc], eax

	mov eax, 63
	mov r11, rbp[-0x8]
	mov r11[0x100], eax

	mov eax, 64
	mov r11, rbp[-0x8]
	mov r11[0x104], eax

	mov eax, 65
	mov r11, rbp[-0x8]
	mov r11[0x108], eax

	mov eax, 66
	mov r11, rbp[-0x8]
	mov r11[0x10c], eax

	mov eax, 67
	mov r11, rbp[-0x8]
	mov r11[0x110], eax

	mov eax, 68
	mov r11, rbp[-0x8]
	mov r11[0x114], eax

	mov eax, 69
	mov r11, rbp[-0x8]
	mov r11[0x118], eax

	mov eax, 70
	mov r11, rbp[-0x8]
	mov r11[0x11c], eax

	mov eax, 71
	mov r11, rbp[-0x8]
	mov r11[0x120], eax

	mov eax, 72
	mov r11, rbp[-0x8]
	mov r11[0x124], eax

	mov eax, 73
	mov r11, rbp[-0x8]
	mov r11[0x128], eax

	mov eax, 74
	mov r11, rbp[-0x8]
	mov r11[0x12c], eax

	mov eax, 75
	mov r11, rbp[-0x8]
	mov r11[0x130], eax

	mov eax, 76
	mov r11, rbp[-0x8]
	mov r11[0x134], eax

	mov eax, 77
	mov r11, rbp[-0x8]
	mov r11[0x138], eax

	mov eax, 78
	mov r11, rbp[-0x8]
	mov r11[0x13c], eax

	mov eax, 79
	mov r11, rbp[-0x8]
	mov r11[0x140], eax

	mov eax, 80
	mov r11, rbp[-0x8]
	mov r11[0x144], eax

	mov eax, 81
	mov r11, rbp[-0x8]
	mov r11[0x148], eax

	mov eax, 82
	mov r11, rbp[-0x8]
	mov r11[0x14c], eax

	mov eax, 83
	mov r11, rbp[-0x8]
	mov r11[0x150], eax

	mov eax, 84
	mov r11, rbp[-0x8]
	mov r11[0x154], eax

	mov eax, 85
	mov r11, rbp[-0x8]
	mov r11[0x158], eax

	mov eax, 86
	mov r11, rbp[-0x8]
	mov r11[0x15c], eax

	mov eax, 87
	mov r11, rbp[-0x8]
	mov r11[0x160], eax

	mov eax, 88
	mov r11, rbp[-0x8]
	mov r11[0x164], eax

	mov eax, 89
	mov r11, rbp[-0x8]
	mov r11[0x168], eax

	mov eax, 90
	mov r11, rbp[-0x8]
	mov r11[0x16c], eax

	mov eax, 91
	mov r11, rbp[-0x8]
	mov r11[0x170], eax

	mov eax, 92
	mov r11, rbp[-0x8]
	mov r11[0x174], eax

	mov eax, 93
	mov r11, rbp[-0x8]
	mov r11[0x178], eax

	mov eax, 94
	mov r11, rbp[-0x8]
	mov r11[0x17c], eax

	mov eax, 95
	mov r11, rbp[-0x8]
	mov r11[0x180], eax

	mov eax, 96
	mov r11, rbp[-0x8]
	mov r11[0x184], eax

	mov eax, 97
	mov r11, rbp[-0x8]
	mov r11[0x188], eax

	mov eax, 98
	mov r11, rbp[-0x8]
	mov r11[0x18c], eax

	mov eax, 99
	mov r11, rbp[-0x8]
	mov r11[0x190], eax

	mov eax, 100
	mov r11, rbp[-0x8]
	mov r11[0x194], eax

	mov eax, 101
	mov r11, rbp[-0x8]
	mov r11[0x198], eax

	mov eax, 102
	mov r11, rbp[-0x8]
	mov r11[0x19c], eax

	mov eax, 103
	mov r11, rbp[-0x8]
	mov r11[0x1a0], eax

	mov eax, 104
	mov r11, rbp[-0x8]
	mov r11[0x1a4], eax

	mov eax, 105
	mov r11, rbp[-0x8]
	mov r11[0x1a8], eax

	mov eax, 106
	mov r11, rbp[-0x8]
	mov r11[0x1ac], eax

	mov eax, 107
	mov r11, rbp[-0x8]
	mov r11[0x1b0], eax

	mov eax, 108
	mov r11, rbp[-0x8]
	mov r11[0x1b4], eax

	mov eax, 109
	mov r11, rbp[-0x8]
	mov r11[0x1b8], eax

	mov eax, 110
	mov r11, rbp[-0x8]
	mov r11[0x1bc], eax

	mov eax, 111
	mov r11, rbp[-0x8]
	mov r11[0x1c0], eax

	mov eax, 112
	mov r11, rbp[-0x8]
	mov r11[0x1c4], eax

	mov eax, 113
	mov r11, rbp[-0x8]
	mov r11[0x1c8], eax

	mov eax, 114
	mov r11, rbp[-0x8]
	mov r11[0x1cc], eax

	mov eax, 115
	mov r11, rbp[-0x8]
	mov r11[0x1d0], eax

	mov eax, 116
	mov r11, rbp[-0x8]
	mov r11[0x1d4], eax

	mov eax, 117
	mov r11, rbp[-0x8]
	mov r11[0x1d8], eax

	mov eax, 118
	mov r11, rbp[-0x8]
	mov r11[0x1dc], eax

	mov eax, 119
	mov r11, rbp[-0x8]
	mov r11[0x1e0], eax

	mov eax, 120
	mov r11, rbp[-0x8]
	mov r11[0x1e4], eax

	mov eax, 121
	mov r11, rbp[-0x8]
	mov r11[0x1e8], eax

	mov eax, 122
	mov r11, rbp[-0x8]
	mov r11[0x1ec], eax

	mov eax, 123
	mov r11, rbp[-0x8]
	mov r11[0x1f0], eax

	mov eax, 124
	mov r11, rbp[-0x8]
	mov r11[0x1f4], eax

	mov eax, 125
	mov r11, rbp[-0x8]
	mov r11[0x1f8], eax

	mov eax, 126
	mov r11, rbp[-0x8]
	mov r11[0x1fc], eax

	mov eax, 127
	mov r11, rbp[-0x8]
	mov r11[0x200], eax

	mov eax, 128
	mov r11, rbp[-0x8]
	mov r11[0x204], eax

	mov eax, 129
	mov r11, rbp[-0x8]
	mov r11[0x208], eax

	mov eax, 130
	mov r11, rbp[-0x8]
	mov r11[0x20c], eax

	mov eax, 131
	mov r11, rbp[-0x8]
	mov r11[0x210], eax

	mov eax, 132
	mov r11, rbp[-0x8]
	mov r11[0x214], eax

	mov eax, 133
	mov r11, rbp[-0x8]
	mov r11[0x218], eax

	mov eax, 134
	mov r11, rbp[-0x8]
	mov r11[0x21c], eax

	mov eax, 135
	mov r11, rbp[-0x8]
	mov r11[0x220], eax

	mov eax, 136
	mov r11, rbp[-0x8]
	mov r11[0x224], eax

	mov eax, 137
	mov r11, rbp[-0x8]
	mov r11[0x228], eax

	mov eax, 138
	mov r11, rbp[-0x8]
	mov r11[0x22c], eax

	mov eax, 139
	mov r11, rbp[-0x8]
	mov r11[0x230], eax

	mov eax, 140
	mov r11, rbp[-0x8]
	mov r11[0x234], eax

	mov eax, 141
	mov r11, rbp[-0x8]
	mov r11[0x238], eax

	mov eax, 142
	mov r11, rbp[-0x8]
	mov r11[0x23c], eax

	mov eax, 143
	mov r11, rbp[-0x8]
	mov r11[0x240], eax

	mov eax, 144
	mov r11, rbp[-0x8]
	mov r11[0x244], eax

	mov eax, 145
	mov r11, rbp[-0x8]
	mov r11[0x248], eax

	mov eax, 146
	mov r11, rbp[-0x8]
	mov r11[0x24c], eax

	mov eax, 147
	mov r11, rbp[-0x8]
	mov r11[0x250], eax

	mov eax, 148
	mov r11, rbp[-0x8]
	mov r11[0x254], eax

	mov eax, 149
	mov r11, rbp[-0x8]
	mov r11[0x258], eax

	mov eax, 150
	mov r11, rbp[-0x8]
	mov r11[0x25c], eax

	mov eax, 151
	mov r11, rbp[-0x8]
	mov r11[0x260], eax

	mov eax, 152
	mov r11, rbp[-0x8]
	mov r11[0x264], eax

	mov eax, 153
	mov r11, rbp[-0x8]
	mov r11[0x268], eax

	mov eax, 154
	mov r11, rbp[-0x8]
	mov r11[0x26c], eax

	mov eax, 155
	mov r11, rbp[-0x8]
	mov r11[0x270], eax

	mov eax, 156
	mov r11, rbp[-0x8]
	mov r11[0x274], eax

	mov eax, 157
	mov r11, rbp[-0x8]
	mov r11[0x278], eax

	mov eax, 158
	mov r11, rbp[-0x8]
	mov r11[0x27c], eax

	mov eax, 159
	mov r11, rbp[-0x8]
	mov r11[0x280], eax

	mov eax, 160
	mov r11, rbp[-0x8]
	mov r11[0x284], eax

	mov eax, 161
	mov r11, rbp[-0x8]
	mov r11[0x288], eax

	mov eax, 162
	mov r11, rbp[-0x8]
	mov r11[0x28c], eax

	mov eax, 163
	mov r11, rbp[-0x8]
	mov r11[0x290], eax

	mov eax, 164
	mov r11, rbp[-0x8]
	mov r11[0x294], eax

	mov eax, 165
	mov r11, rbp[-0x8]
	mov r11[0x298], eax

	mov eax, 166
	mov r11, rbp[-0x8]
	mov r11[0x29c], eax

	mov eax, 167
	mov r11, rbp[-0x8]
	mov r11[0x2a0], eax

	mov eax, 168
	mov r11, rbp[-0x8]
	mov r11[0x2a4], eax

	mov eax, 169
	mov r11, rbp[-0x8]
	mov r11[0x2a8], eax

	mov eax, 170
	mov r11, rbp[-0x8]
	mov r11[0x2ac], eax

	mov eax, 171
	mov r11, rbp[-0x8]
	mov r11[0x2b0], eax

	mov eax, 172
	mov r11, rbp[-0x8]
	mov r11[0x2b4], eax

	mov eax, 173
	mov r11, rbp[-0x8]
	mov r11[0x2b8], eax

	mov eax, 174
	mov r11, rbp[-0x8]
	mov r11[0x2bc], eax

	mov eax, 175
	mov r11, rbp[-0x8]
	mov r11[0x2c0], eax

	mov eax, 176
	mov r11, rbp[-0x8]
	mov r11[0x2c4], eax

	mov eax, 177
	mov r11, rbp[-0x8]
	mov r11[0x2c8], eax

	mov eax, 178
	mov r11, rbp[-0x8]
	mov r11[0x2cc], eax

	mov eax, 179
	mov r11, rbp[-0x8]
	mov r11[0x2d0], eax

	mov eax, 180
	mov r11, rbp[-0x8]
	mov r11[0x2d4], eax

	mov eax, 181
	mov r11, rbp[-0x8]
	mov r11[0x2d8], eax

	mov eax, 182
	mov r11, rbp[-0x8]
	mov r11[0x2dc], eax

	mov eax, 183
	mov r11, rbp[-0x8]
	mov r11[0x2e0], eax

	mov eax, 184
	mov r11, rbp[-0x8]
	mov r11[0x2e4], eax

	mov eax, 185
	mov r11, rbp[-0x8]
	mov r11[0x2e8], eax

	mov eax, 186
	mov r11, rbp[-0x8]
	mov r11[0x2ec], eax

	mov eax, 187
	mov r11, rbp[-0x8]
	mov r11[0x2f0], eax

	mov eax, 188
	mov r11, rbp[-0x8]
	mov r11[0x2f4], eax

	mov eax, 189
	mov r11, rbp[-0x8]
	mov r11[0x2f8], eax

	mov eax, 190
	mov r11, rbp[-0x8]
	mov r11[0x2fc], eax

	mov eax, 191
	mov r11, rbp[-0x8]
	mov r11[0x300], eax

	mov eax, 192
	mov r11, rbp[-0x8]
	mov r11[0x304], eax

	mov eax, 193
	mov r11, rbp[-0x8]
	mov r11[0x308], eax

	mov eax, 194
	mov r11, rbp[-0x8]
	mov r11[0x30c], eax

	mov eax, 195
	mov r11, rbp[-0x8]
	mov r11[0x310], eax

	mov eax, 196
	mov r11, rbp[-0x8]
	mov r11[0x314], eax

	mov eax, 197
	mov r11, rbp[-0x8]
	mov r11[0x318], eax

	mov eax, 198
	mov r11, rbp[-0x8]
	mov r11[0x31c], eax

	mov eax, 199
	mov r11, rbp[-0x8]
	mov r11[0x320], eax

	mov eax, 200
	mov r11, rbp[-0x8]
	mov r11[0x324], eax

	mov eax, 201
	mov r11, rbp[-0x8]
	mov r11[0x328], eax

	mov eax, 202
	mov r11, rbp[-0x8]
	mov r11[0x32c], eax

	mov eax, 203
	mov r11, rbp[-0x8]
	mov r11[0x330], eax

	mov eax, 204
	mov r11, rbp[-0x8]
	mov r11[0x334], eax

	mov eax, 205
	mov r11, rbp[-0x8]
	mov r11[0x338], eax

	mov eax, 206
	mov r11, rbp[-0x8]
	mov r11[0x33c], eax

	mov eax, 207
	mov r11, rbp[-0x8]
	mov r11[0x340], eax

	mov eax, 208
	mov r11, rbp[-0x8]
	mov r11[0x344], eax

	mov eax, 209
	mov r11, rbp[-0x8]
	mov r11[0x348], eax

	mov eax, 210
	mov r11, rbp[-0x8]
	mov r11[0x34c], eax

	mov eax, 211
	mov r11, rbp[-0x8]
	mov r11[0x350], eax

	mov eax, 212
	mov r11, rbp[-0x8]
	mov r11[0x354], eax

	mov eax, 213
	mov r11, rbp[-0x8]
	mov r11[0x358], eax

	mov eax, 214
	mov r11, rbp[-0x8]
	mov r11[0x35c], eax

	mov eax, 215
	mov r11, rbp[-0x8]
	mov r11[0x360], eax

	mov eax, 216
	mov r11, rbp[-0x8]
	mov r11[0x364], eax

	mov eax, 217
	mov r11, rbp[-0x8]
	mov r11[0x368], eax

	mov eax, 218
	mov r11, rbp[-0x8]
	mov r11[0x36c], eax

	mov eax, 219
	mov r11, rbp[-0x8]
	mov r11[0x370], eax

	mov eax, 220
	mov r11, rbp[-0x8]
	mov r11[0x374], eax

	mov eax, 221
	mov r11, rbp[-0x8]
	mov r11[0x378], eax

	mov eax, 222
	mov r11, rbp[-0x8]
	mov r11[0x37c], eax

	mov eax, 223
	mov r11, rbp[-0x8]
	mov r11[0x380], eax

	mov eax, 224
	mov r11, rbp[-0x8]
	mov r11[0x384], eax

	mov eax, 225
	mov r11, rbp[-0x8]
	mov r11[0x388], eax

	mov eax, 226
	mov r11, rbp[-0x8]
	mov r11[0x38c], eax

	mov eax, 227
	mov r11, rbp[-0x8]
	mov r11[0x390], eax

	mov eax, 228
	mov r11, rbp[-0x8]
	mov r11[0x394], eax

	mov eax, 229
	mov r11, rbp[-0x8]
	mov r11[0x398], eax

	mov eax, 230
	mov r11, rbp[-0x8]
	mov r11[0x39c], eax

	mov eax, 231
	mov r11, rbp[-0x8]
	mov r11[0x3a0], eax

	mov eax, 232
	mov r11, rbp[-0x8]
	mov r11[0x3a4], eax

	mov eax, 233
	mov r11, rbp[-0x8]
	mov r11[0x3a8], eax

	mov eax, 234
	mov r11, rbp[-0x8]
	mov r11[0x3ac], eax

	mov eax, 235
	mov r11, rbp[-0x8]
	mov r11[0x3b0], eax

	mov eax, 236
	mov r11, rbp[-0x8]
	mov r11[0x3b4], eax

	mov eax, 237
	mov r11, rbp[-0x8]
	mov r11[0x3b8], eax

	mov eax, 238
	mov r11, rbp[-0x8]
	mov r11[0x3bc], eax

	mov eax, 239
	mov r11, rbp[-0x8]
	mov r11[0x3c0], eax

	mov eax, 240
	mov r11, rbp[-0x8]
	mov r11[0x3c4], eax

	mov eax, 241
	mov r11, rbp[-0x8]
	mov r11[0x3c8], eax

	mov eax, 242
	mov r11, rbp[-0x8]
	mov r11[0x3cc], eax

	mov eax, 243
	mov r11, rbp[-0x8]
	mov r11[0x3d0], eax

	mov eax, 244
	mov r11, rbp[-0x8]
	mov r11[0x3d4], eax

	mov eax, 245
	mov r11, rbp[-0x8]
	mov r11[0x3d8], eax

	mov eax, 246
	mov r11, rbp[-0x8]
	mov r11[0x3dc], eax

	mov eax, 247
	mov r11, rbp[-0x8]
	mov r11[0x3e0], eax

	mov eax, 248
	mov r11, rbp[-0x8]
	mov r11[0x3e4], eax

	mov eax, 249
	mov r11, rbp[-0x8]
	mov r11[0x3e8], eax

	mov eax, 250
	mov r11, rbp[-0x8]
	mov r11[0x3ec], eax

	mov eax, 251
	mov r11, rbp[-0x8]
	mov r11[0x3f0], eax

	mov eax, 252
	mov r11, rbp[-0x8]
	mov r11[0x3f4], eax

	mov eax, 253
	mov r11, rbp[-0x8]
	mov r11[0x3f8], eax

	mov eax, 254
	mov r11, rbp[-0x8]
	mov r11[0x3fc], eax

	mov eax, 255
	mov r11, rbp[-0x8]
	mov r11[0x400], eax

	mov eax, 256
	mov r11, rbp[-0x8]
	mov r11[0x404], eax

	mov eax, 257
	mov r11, rbp[-0x8]
	mov r11[0x408], eax

	mov eax, 258
	mov r11, rbp[-0x8]
	mov r11[0x40c], eax

	mov eax, 259
	mov r11, rbp[-0x8]
	mov r11[0x410], eax

	mov eax, 260
	mov r11, rbp[-0x8]
	mov r11[0x414], eax

	mov eax, 261
	mov r11, rbp[-0x8]
	mov r11[0x418], eax

	mov eax, 262
	mov r11, rbp[-0x8]
	mov r11[0x41c], eax

	mov eax, 263
	mov r11, rbp[-0x8]
	mov r11[0x420], eax

	mov eax, 264
	mov r11, rbp[-0x8]
	mov r11[0x424], eax

	mov eax, 265
	mov r11, rbp[-0x8]
	mov r11[0x428], eax

	mov eax, 266
	mov r11, rbp[-0x8]
	mov r11[0x42c], eax

	mov eax, 267
	mov r11, rbp[-0x8]
	mov r11[0x430], eax

	mov eax, 268
	mov r11, rbp[-0x8]
	mov r11[0x434], eax

	mov eax, 269
	mov r11, rbp[-0x8]
	mov r11[0x438], eax

	mov eax, 270
	mov r11, rbp[-0x8]
	mov r11[0x43c], eax

	mov eax, 271
	mov r11, rbp[-0x8]
	mov r11[0x440], eax

	mov eax, 272
	mov r11, rbp[-0x8]
	mov r11[0x444], eax

	mov eax, 273
	mov r11, rbp[-0x8]
	mov r11[0x448], eax

	mov eax, 274
	mov r11, rbp[-0x8]
	mov r11[0x44c], eax

	mov eax, 275
	mov r11, rbp[-0x8]
	mov r11[0x450], eax

	mov eax, 276
	mov r11, rbp[-0x8]
	mov r11[0x454], eax

	mov eax, 277
	mov r11, rbp[-0x8]
	mov r11[0x458], eax

	mov eax, 278
	mov r11, rbp[-0x8]
	mov r11[0x45c], eax

	mov eax, 279
	mov r11, rbp[-0x8]
	mov r11[0x460], eax

	mov eax, 280
	mov r11, rbp[-0x8]
	mov r11[0x464], eax

	mov eax, 281
	mov r11, rbp[-0x8]
	mov r11[0x468], eax

	mov eax, 282
	mov r11, rbp[-0x8]
	mov r11[0x46c], eax

	mov eax, 283
	mov r11, rbp[-0x8]
	mov r11[0x470], eax

	mov eax, 284
	mov r11, rbp[-0x8]
	mov r11[0x474], eax

	mov eax, 285
	mov r11, rbp[-0x8]
	mov r11[0x478], eax

	mov eax, 286
	mov r11, rbp[-0x8]
	mov r11[0x47c], eax

	mov eax, 287
	mov r11, rbp[-0x8]
	mov r11[0x480], eax

	mov eax, 288
	mov r11, rbp[-0x8]
	mov r11[0x484], eax

	mov eax, 289
	mov r11, rbp[-0x8]
	mov r11[0x488], eax

	mov eax, 290
	mov r11, rbp[-0x8]
	mov r11[0x48c], eax

	mov eax, 291
	mov r11, rbp[-0x8]
	mov r11[0x490], eax

	mov eax, 292
	mov r11, rbp[-0x8]
	mov r11[0x494], eax

	mov eax, 293
	mov r11, rbp[-0x8]
	mov r11[0x498], eax

	mov eax, 294
	mov r11, rbp[-0x8]
	mov r11[0x49c], eax

	mov eax, 295
	mov r11, rbp[-0x8]
	mov r11[0x4a0], eax

	mov eax, 296
	mov r11, rbp[-0x8]
	mov r11[0x4a4], eax

	mov eax, 297
	mov r11, rbp[-0x8]
	mov r11[0x4a8], eax

	mov eax, 298
	mov r11, rbp[-0x8]
	mov r11[0x4ac], eax

	mov eax, 299
	mov r11, rbp[-0x8]
	mov r11[0x4b0], eax

	mov eax, 300
	mov r11, rbp[-0x8]
	mov r11[0x4b4], eax

	mov eax, 301
	mov r11, rbp[-0x8]
	mov r11[0x4b8], eax

	mov eax, 302
	mov r11, rbp[-0x8]
	mov r11[0x4bc], eax

	mov eax, 303
	mov r11, rbp[-0x8]
	mov r11[0x4c0], eax

	mov eax, 304
	mov r11, rbp[-0x8]
	mov r11[0x4c4], eax

	mov eax, 305
	mov r11, rbp[-0x8]
	mov r11[0x4c8], eax

	mov eax, 306
	mov r11, rbp[-0x8]
	mov r11[0x4cc], eax

	mov eax, 307
	mov r11, rbp[-0x8]
	mov r11[0x4d0], eax

	mov eax, 308
	mov r11, rbp[-0x8]
	mov r11[0x4d4], eax

	mov eax, 309
	mov r11, rbp[-0x8]
	mov r11[0x4d8], eax

	mov eax, 310
	mov r11, rbp[-0x8]
	mov r11[0x4dc], eax

	mov eax, 311
	mov r11, rbp[-0x8]
	mov r11[0x4e0], eax

	mov eax, 312
	mov r11, rbp[-0x8]
	mov r11[0x4e4], eax

	mov eax, 313
	mov r11, rbp[-0x8]
	mov r11[0x4e8], eax

	mov eax, 314
	mov r11, rbp[-0x8]
	mov r11[0x4ec], eax

	mov eax, 315
	mov r11, rbp[-0x8]
	mov r11[0x4f0], eax

	mov eax, 316
	mov r11, rbp[-0x8]
	mov r11[0x4f4], eax

	mov eax, 317
	mov r11, rbp[-0x8]
	mov r11[0x4f8], eax

	mov eax, 318
	mov r11, rbp[-0x8]
	mov r11[0x4fc], eax

	mov eax, 319
	mov r11, rbp[-0x8]
	mov r11[0x500], eax

	mov eax, 320
	mov r11, rbp[-0x8]
	mov r11[0x504], eax

	mov eax, 321
	mov r11, rbp[-0x8]
	mov r11[0x508], eax

	mov eax, 322
	mov r11, rbp[-0x8]
	mov r11[0x50c], eax

	mov eax, 323
	mov r11, rbp[-0x8]
	mov r11[0x510], eax

	mov eax, 324
	mov r11, rbp[-0x8]
	mov r11[0x514], eax

	mov eax, 325
	mov r11, rbp[-0x8]
	mov r11[0x518], eax

	mov eax, 326
	mov r11, rbp[-0x8]
	mov r11[0x51c], eax

	mov eax, 327
	mov r11, rbp[-0x8]
	mov r11[0x520], eax

	mov eax, 328
	mov r11, rbp[-0x8]
	mov r11[0x524], eax

	mov eax, 329
	mov r11, rbp[-0x8]
	mov r11[0x528], eax

	mov eax, 330
	mov r11, rbp[-0x8]
	mov r11[0x52c], eax

	mov eax, 331
	mov r11, rbp[-0x8]
	mov r11[0x530], eax

	mov eax, 332
	mov r11, rbp[-0x8]
	mov r11[0x534], eax

	mov eax, 333
	mov r11, rbp[-0x8]
	mov r11[0x538], eax

	mov eax, 334
	mov r11, rbp[-0x8]
	mov r11[0x53c], eax

	mov eax, 335
	mov r11, rbp[-0x8]
	mov r11[0x540], eax

	mov eax, 336
	mov r11, rbp[-0x8]
	mov r11[0x544], eax

	mov eax, 337
	mov r11, rbp[-0x8]
	mov r11[0x548], eax

	mov eax, 338
	mov r11, rbp[-0x8]
	mov r11[0x54c], eax

	mov eax, 339
	mov r11, rbp[-0x8]
	mov r11[0x550], eax

	mov eax, 340
	mov r11, rbp[-0x8]
	mov r11[0x554], eax

	mov eax, 341
	mov r11, rbp[-0x8]
	mov r11[0x558], eax

	mov eax, 342
	mov r11, rbp[-0x8]
	mov r11[0x55c], eax

	mov eax, 343
	mov r11, rbp[-0x8]
	mov r11[0x560], eax

	mov eax, 344
	mov r11, rbp[-0x8]
	mov r11[0x564], eax

	mov eax, 345
	mov r11, rbp[-0x8]
	mov r11[0x568], eax

	mov eax, 346
	mov r11, rbp[-0x8]
	mov r11[0x56c], eax

	mov eax, 347
	mov r11, rbp[-0x8]
	mov r11[0x570], eax

	mov eax, 348
	mov r11, rbp[-0x8]
	mov r11[0x574], eax

	mov eax, 349
	mov r11, rbp[-0x8]
	mov r11[0x578], eax

	mov eax, 350
	mov r11, rbp[-0x8]
	mov r11[0x57c], eax

	mov eax, 351
	mov r11, rbp[-0x8]
	mov r11[0x580], eax

	mov eax, 352
	mov r11, rbp[-0x8]
	mov r11[0x584], eax

	mov eax, 353
	mov r11, rbp[-0x8]
	mov r11[0x588], eax

	mov eax, 354
	mov r11, rbp[-0x8]
	mov r11[0x58c], eax

	mov eax, 355
	mov r11, rbp[-0x8]
	mov r11[0x590], eax

	mov eax, 356
	mov r11, rbp[-0x8]
	mov r11[0x594], eax

	mov eax, 357
	mov r11, rbp[-0x8]
	mov r11[0x598], eax

	mov eax, 358
	mov r11, rbp[-0x8]
	mov r11[0x59c], eax

	mov eax, 359
	mov r11, rbp[-0x8]
	mov r11[0x5a0], eax

	mov eax, 360
	mov r11, rbp[-0x8]
	mov r11[0x5a4], eax

	mov eax, 361
	mov r11, rbp[-0x8]
	mov r11[0x5a8], eax

	mov eax, 362
	mov r11, rbp[-0x8]
	mov r11[0x5ac], eax

	mov eax, 363
	mov r11, rbp[-0x8]
	mov r11[0x5b0], eax

	mov eax, 364
	mov r11, rbp[-0x8]
	mov r11[0x5b4], eax

	mov eax, 365
	mov r11, rbp[-0x8]
	mov r11[0x5b8], eax

	mov eax, 366
	mov r11, rbp[-0x8]
	mov r11[0x5bc], eax

	mov eax, 367
	mov r11, rbp[-0x8]
	mov r11[0x5c0], eax

	mov eax, 368
	mov r11, rbp[-0x8]
	mov r11[0x5c4], eax

	mov eax, 369
	mov r11, rbp[-0x8]
	mov r11[0x5c8], eax

	mov eax, 370
	mov r11, rbp[-0x8]
	mov r11[0x5cc], eax

	mov eax, 371
	mov r11, rbp[-0x8]
	mov r11[0x5d0], eax

	mov eax, 372
	mov r11, rbp[-0x8]
	mov r11[0x5d4], eax

	mov eax, 373
	mov r11, rbp[-0x8]
	mov r11[0x5d8], eax

	mov eax, 374
	mov r11, rbp[-0x8]
	mov r11[0x5dc], eax

	mov eax, 375
	mov r11, rbp[-0x8]
	mov r11[0x5e0], eax

	mov eax, 376
	mov r11, rbp[-0x8]
	mov r11[0x5e4], eax

	mov eax, 377
	mov r11, rbp[-0x8]
	mov r11[0x5e8], eax

	mov eax, 378
	mov r11, rbp[-0x8]
	mov r11[0x5ec], eax

	mov eax, 379
	mov r11, rbp[-0x8]
	mov r11[0x5f0], eax

	mov eax, 380
	mov r11, rbp[-0x8]
	mov r11[0x5f4], eax

	mov eax, 381
	mov r11, rbp[-0x8]
	mov r11[0x5f8], eax

	mov eax, 382
	mov r11, rbp[-0x8]
	mov r11[0x5fc], eax

	mov eax, 383
	mov r11, rbp[-0x8]
	mov r11[0x600], eax

	mov eax, 384
	mov r11, rbp[-0x8]
	mov r11[0x604], eax

	mov eax, 385
	mov r11, rbp[-0x8]
	mov r11[0x608], eax

	mov eax, 386
	mov r11, rbp[-0x8]
	mov r11[0x60c], eax

	mov eax, 387
	mov r11, rbp[-0x8]
	mov r11[0x610], eax

	mov eax, 388
	mov r11, rbp[-0x8]
	mov r11[0x614], eax

	mov eax, 389
	mov r11, rbp[-0x8]
	mov r11[0x618], eax

	mov eax, 390
	mov r11, rbp[-0x8]
	mov r11[0x61c], eax

	mov eax, 391
	mov r11, rbp[-0x8]
	mov r11[0x620], eax

	mov eax, 392
	mov r11, rbp[-0x8]
	mov r11[0x624], eax

	mov eax, 393
	mov r11, rbp[-0x8]
	mov r11[0x628], eax

	mov eax, 394
	mov r11, rbp[-0x8]
	mov r11[0x62c], eax

	mov eax, 395
	mov r11, rbp[-0x8]
	mov r11[0x630], eax

	mov eax, 396
	mov r11, rbp[-0x8]
	mov r11[0x634], eax

	mov eax, 397
	mov r11, rbp[-0x8]
	mov r11[0x638], eax

	mov eax, 398
	mov r11, rbp[-0x8]
	mov r11[0x63c], eax

	mov eax, 399
	mov r11, rbp[-0x8]
	mov r11[0x640], eax

	mov eax, 400
	mov r11, rbp[-0x8]
	mov r11[0x644], eax

	mov eax, 401
	mov r11, rbp[-0x8]
	mov r11[0x648], eax

	mov eax, 402
	mov r11, rbp[-0x8]
	mov r11[0x64c], eax

	mov eax, 403
	mov r11, rbp[-0x8]
	mov r11[0x650], eax

	mov eax, 404
	mov r11, rbp[-0x8]
	mov r11[0x654], eax

	mov eax, 405
	mov r11, rbp[-0x8]
	mov r11[0x658], eax

	mov eax, 406
	mov r11, rbp[-0x8]
	mov r11[0x65c], eax

	mov eax, 407
	mov r11, rbp[-0x8]
	mov r11[0x660], eax

	mov eax, 408
	mov r11, rbp[-0x8]
	mov r11[0x664], eax

	mov eax, 409
	mov r11, rbp[-0x8]
	mov r11[0x668], eax

	mov eax, 410
	mov r11, rbp[-0x8]
	mov r11[0x66c], eax

	mov eax, 411
	mov r11, rbp[-0x8]
	mov r11[0x670], eax

	mov eax, 412
	mov r11, rbp[-0x8]
	mov r11[0x674], eax

	mov eax, 413
	mov r11, rbp[-0x8]
	mov r11[0x678], eax

	mov eax, 414
	mov r11, rbp[-0x8]
	mov r11[0x67c], eax

	mov eax, 415
	mov r11, rbp[-0x8]
	mov r11[0x680], eax

	mov eax, 416
	mov r11, rbp[-0x8]
	mov r11[0x684], eax

	mov eax, 417
	mov r11, rbp[-0x8]
	mov r11[0x688], eax

	mov eax, 418
	mov r11, rbp[-0x8]
	mov r11[0x68c], eax

	mov eax, 419
	mov r11, rbp[-0x8]
	mov r11[0x690], eax

	mov eax, 420
	mov r11, rbp[-0x8]
	mov r11[0x694], eax

	mov eax, 421
	mov r11, rbp[-0x8]
	mov r11[0x698], eax

	mov eax, 422
	mov r11, rbp[-0x8]
	mov r11[0x69c], eax

	mov eax, 423
	mov r11, rbp[-0x8]
	mov r11[0x6a0], eax

	mov eax, 424
	mov r11, rbp[-0x8]
	mov r11[0x6a4], eax

	mov eax, 425
	mov r11, rbp[-0x8]
	mov r11[0x6a8], eax

	mov eax, 426
	mov r11, rbp[-0x8]
	mov r11[0x6ac], eax

	mov eax, 427
	mov r11, rbp[-0x8]
	mov r11[0x6b0], eax

	mov eax, 428
	mov r11, rbp[-0x8]
	mov r11[0x6b4], eax

	mov eax, 429
	mov r11, rbp[-0x8]
	mov r11[0x6b8], eax

	mov eax, 430
	mov r11, rbp[-0x8]
	mov r11[0x6bc], eax

	mov eax, 431
	mov r11, rbp[-0x8]
	mov r11[0x6c0], eax

	mov eax, 432
	mov r11, rbp[-0x8]
	mov r11[0x6c4], eax

	mov eax, 433
	mov r11, rbp[-0x8]
	mov r11[0x6c8], eax

	mov eax, 434
	mov r11, rbp[-0x8]
	mov r11[0x6cc], eax

	mov eax, 435
	mov r11, rbp[-0x8]
	mov r11[0x6d0], eax

	mov eax, 436
	mov r11, rbp[-0x8]
	mov r11[0x6d4], eax

	mov eax, 437
	mov r11, rbp[-0x8]
	mov r11[0x6d8], eax

	mov eax, 438
	mov r11, rbp[-0x8]
	mov r11[0x6dc], eax

	mov eax, 439
	mov r11, rbp[-0x8]
	mov r11[0x6e0], eax

	mov eax, 440
	mov r11, rbp[-0x8]
	mov r11[0x6e4], eax

	mov eax, 441
	mov r11, rbp[-0x8]
	mov r11[0x6e8], eax

	mov eax, 442
	mov r11, rbp[-0x8]
	mov r11[0x6ec], eax

	mov eax, 443
	mov r11, rbp[-0x8]
	mov r11[0x6f0], eax

	mov eax, 444
	mov r11, rbp[-0x8]
	mov r11[0x6f4], eax

	mov eax, 445
	mov r11, rbp[-0x8]
	mov r11[0x6f8], eax

	mov eax, 446
	mov r11, rbp[-0x8]
	mov r11[0x6fc], eax

	mov eax, 447
	mov r11, rbp[-0x8]
	mov r11[0x700], eax

	mov eax, 448
	mov r11, rbp[-0x8]
	mov r11[0x704], eax

	mov eax, 449
	mov r11, rbp[-0x8]
	mov r11[0x708], eax

	mov eax, 450
	mov r11, rbp[-0x8]
	mov r11[0x70c], eax

	mov eax, 451
	mov r11, rbp[-0x8]
	mov r11[0x710], eax

	mov eax, 452
	mov r11, rbp[-0x8]
	mov r11[0x714], eax

	mov eax, 453
	mov r11, rbp[-0x8]
	mov r11[0x718], eax

	mov eax, 454
	mov r11, rbp[-0x8]
	mov r11[0x71c], eax

	mov eax, 455
	mov r11, rbp[-0x8]
	mov r11[0x720], eax

	mov eax, 456
	mov r11, rbp[-0x8]
	mov r11[0x724], eax

	mov eax, 457
	mov r11, rbp[-0x8]
	mov r11[0x728], eax

	mov eax, 458
	mov r11, rbp[-0x8]
	mov r11[0x72c], eax

	mov eax, 459
	mov r11, rbp[-0x8]
	mov r11[0x730], eax

	mov eax, 460
	mov r11, rbp[-0x8]
	mov r11[0x734], eax

	mov eax, 461
	mov r11, rbp[-0x8]
	mov r11[0x738], eax

	mov eax, 462
	mov r11, rbp[-0x8]
	mov r11[0x73c], eax

	mov eax, 463
	mov r11, rbp[-0x8]
	mov r11[0x740], eax

	mov eax, 464
	mov r11, rbp[-0x8]
	mov r11[0x744], eax

	mov eax, 465
	mov r11, rbp[-0x8]
	mov r11[0x748], eax

	mov eax, 466
	mov r11, rbp[-0x8]
	mov r11[0x74c], eax

	mov eax, 467
	mov r11, rbp[-0x8]
	mov r11[0x750], eax

	mov eax, 468
	mov r11, rbp[-0x8]
	mov r11[0x754], eax

	mov eax, 469
	mov r11, rbp[-0x8]
	mov r11[0x758], eax

	mov eax, 470
	mov r11, rbp[-0x8]
	mov r11[0x75c], eax

	mov eax, 471
	mov r11, rbp[-0x8]
	mov r11[0x760], eax

	mov eax, 472
	mov r11, rbp[-0x8]
	mov r11[0x764], eax

	mov eax, 473
	mov r11, rbp[-0x8]
	mov r11[0x768], eax

	mov eax, 474
	mov r11, rbp[-0x8]
	mov r11[0x76c], eax

	mov eax, 475
	mov r11, rbp[-0x8]
	mov r11[0x770], eax

	mov eax, 476
	mov r11, rbp[-0x8]
	mov r11[0x774], eax

	mov eax, 477
	mov r11, rbp[-0x8]
	mov r11[0x778], eax

	mov eax, 478
	mov r11, rbp[-0x8]
	mov r11[0x77c], eax

	mov eax, 479
	mov r11, rbp[-0x8]
	mov r11[0x780], eax

	mov eax, 480
	mov r11, rbp[-0x8]
	mov r11[0x784], eax

	mov eax, 481
	mov r11, rbp[-0x8]
	mov r11[0x788], eax

	mov eax, 482
	mov r11, rbp[-0x8]
	mov r11[0x78c], eax

	mov eax, 483
	mov r11, rbp[-0x8]
	mov r11[0x790], eax

	mov eax, 484
	mov r11, rbp[-0x8]
	mov r11[0x794], eax

	mov eax, 485
	mov r11, rbp[-0x8]
	mov r11[0x798], eax

	mov eax, 486
	mov r11, rbp[-0x8]
	mov r11[0x79c], eax

	mov eax, 487
	mov r11, rbp[-0x8]
	mov r11[0x7a0], eax

	mov eax, 488
	mov r11, rbp[-0x8]
	mov r11[0x7a4], eax

	mov eax, 489
	mov r11, rbp[-0x8]
	mov r11[0x7a8], eax

	mov eax, 490
	mov r11, rbp[-0x8]
	mov r11[0x7ac], eax

	mov eax, 491
	mov r11, rbp[-0x8]
	mov r11[0x7b0], eax

	mov eax, 492
	mov r11, rbp[-0x8]
	mov r11[0x7b4], eax

	mov eax, 493
	mov r11, rbp[-0x8]
	mov r11[0x7b8], eax

	mov eax, 494
	mov r11, rbp[-0x8]
	mov r11[0x7bc], eax

	mov eax, 495
	mov r11, rbp[-0x8]
	mov r11[0x7c0], eax

	mov eax, 496
	mov r11, rbp[-0x8]
	mov r11[0x7c4], eax

	mov eax, 497
	mov r11, rbp[-0x8]
	mov r11[0x7c8], eax

	mov eax, 498
	mov r11, rbp[-0x8]
	mov r11[0x7cc], eax

	mov eax, 499
	mov r11, rbp[-0x8]
	mov r11[0x7d0], eax

	mov eax, 500
	mov r11, rbp[-0x8]
	mov r11[0x7d4], eax

	mov eax, 501
	mov r11, rbp[-0x8]
	mov r11[0x7d8], eax

	mov eax, 502
	mov r11, rbp[-0x8]
	mov r11[0x7dc], eax

	mov eax, 503
	mov r11, rbp[-0x8]
	mov r11[0x7e0], eax

	mov eax, 504
	mov r11, rbp[-0x8]
	mov r11[0x7e4], eax

	mov eax, 505
	mov r11, rbp[-0x8]
	mov r11[0x7e8], eax

	mov eax, 506
	mov r11, rbp[-0x8]
	mov r11[0x7ec], eax

	mov eax, 507
	mov r11, rbp[-0x8]
	mov r11[0x7f0], eax

	mov eax, 508
	mov r11, rbp[-0x8]
	mov r11[0x7f4], eax

	mov eax, 509
	mov r11, rbp[-0x8]
	mov r11[0x7f8], eax

	mov eax, 510
	mov r11, rbp[-0x8]
	mov r11[0x7fc], eax

	mov eax, 511
	mov r11, rbp[-0x8]
	mov r11[0x800], eax

	mov eax, 512
	mov r11, rbp[-0x8]
	mov r11[0x804], eax

	mov eax, 513
	mov r11, rbp[-0x8]
	mov r11[0x808], eax

	mov eax, 514
	mov r11, rbp[-0x8]
	mov r11[0x80c], eax

	mov eax, 515
	mov r11, rbp[-0x8]
	mov r11[0x810], eax

	mov eax, 516
	mov r11, rbp[-0x8]
	mov r11[0x814], eax

	mov eax, 517
	mov r11, rbp[-0x8]
	mov r11[0x818], eax

	mov eax, 518
	mov r11, rbp[-0x8]
	mov r11[0x81c], eax

	mov eax, 519
	mov r11, rbp[-0x8]
	mov r11[0x820], eax

	mov eax, 520
	mov r11, rbp[-0x8]
	mov r11[0x824], eax

	mov eax, 521
	mov r11, rbp[-0x8]
	mov r11[0x828], eax

	mov eax, 522
	mov r11, rbp[-0x8]
	mov r11[0x82c], eax

	mov eax, 523
	mov r11, rbp[-0x8]
	mov r11[0x830], eax

	mov eax, 524
	mov r11, rbp[-0x8]
	mov r11[0x834], eax

	mov eax, 525
	mov r11, rbp[-0x8]
	mov r11[0x838], eax

	mov eax, 526
	mov r11, rbp[-0x8]
	mov r11[0x83c], eax

	mov eax, 527
	mov r11, rbp[-0x8]
	mov r11[0x840], eax

	mov eax, 528
	mov r11, rbp[-0x8]
	mov r11[0x844], eax

	mov eax, 529
	mov r11, rbp[-0x8]
	mov r11[0x848], eax

	mov eax, 530
	mov r11, rbp[-0x8]
	mov r11[0x84c], eax

	mov eax, 531
	mov r11, rbp[-0x8]
	mov r11[0x850], eax

	mov eax, 532
	mov r11, rbp[-0x8]
	mov r11[0x854], eax

	mov eax, 533
	mov r11, rbp[-0x8]
	mov r11[0x858], eax

	mov eax, 534
	mov r11, rbp[-0x8]
	mov r11[0x85c], eax

	mov eax, 535
	mov r11, rbp[-0x8]
	mov r11[0x860], eax

	mov eax, 536
	mov r11, rbp[-0x8]
	mov r11[0x864], eax

	mov eax, 537
	mov r11, rbp[-0x8]
	mov r11[0x868], eax

	mov eax, 538
	mov r11, rbp[-0x8]
	mov r11[0x86c], eax

	mov eax, 539
	mov r11, rbp[-0x8]
	mov r11[0x870], eax

	mov eax, 540
	mov r11, rbp[-0x8]
	mov r11[0x874], eax

	mov eax, 541
	mov r11, rbp[-0x8]
	mov r11[0x878], eax

	mov eax, 542
	mov r11, rbp[-0x8]
	mov r11[0x87c], eax

	mov eax, 543
	mov r11, rbp[-0x8]
	mov r11[0x880], eax

	mov eax, 544
	mov r11, rbp[-0x8]
	mov r11[0x884], eax

	mov eax, 545
	mov r11, rbp[-0x8]
	mov r11[0x888], eax

	mov eax, 546
	mov r11, rbp[-0x8]
	mov r11[0x88c], eax

	mov eax, 547
	mov r11, rbp[-0x8]
	mov r11[0x890], eax

	mov eax, 548
	mov r11, rbp[-0x8]
	mov r11[0x894], eax

	mov eax, 549
	mov r11, rbp[-0x8]
	mov r11[0x898], eax

	mov eax, 550
	mov r11, rbp[-0x8]
	mov r11[0x89c], eax

	mov eax, 551
	mov r11, rbp[-0x8]
	mov r11[0x8a0], eax

	mov eax, 552
	mov r11, rbp[-0x8]
	mov r11[0x8a4], eax

	mov eax, 553
	mov r11, rbp[-0x8]
	mov r11[0x8a8], eax

	mov eax, 554
	mov r11, rbp[-0x8]
	mov r11[0x8ac], eax

	mov eax, 555
	mov r11, rbp[-0x8]
	mov r11[0x8b0], eax

	mov eax, 556
	mov r11, rbp[-0x8]
	mov r11[0x8b4], eax

	mov eax, 557
	mov r11, rbp[-0x8]
	mov r11[0x8b8], eax

	mov eax, 558
	mov r11, rbp[-0x8]
	mov r11[0x8bc], eax

	mov eax, 559
	mov r11, rbp[-0x8]
	mov r11[0x8c0], eax

	mov eax, 560
	mov r11, rbp[-0x8]
	mov r11[0x8c4], eax

	mov eax, 561
	mov r11, rbp[-0x8]
	mov r11[0x8c8], eax

	mov eax, 562
	mov r11, rbp[-0x8]
	mov r11[0x8cc], eax

	mov eax, 563
	mov r11, rbp[-0x8]
	mov r11[0x8d0], eax

	mov eax, 564
	mov r11, rbp[-0x8]
	mov r11[0x8d4], eax

	mov eax, 565
	mov r11, rbp[-0x8]
	mov r11[0x8d8], eax

	mov eax, 566
	mov r11, rbp[-0x8]
	mov r11[0x8dc], eax

	mov eax, 567
	mov r11, rbp[-0x8]
	mov r11[0x8e0], eax

	mov eax, 568
	mov r11, rbp[-0x8]
	mov r11[0x8e4], eax

	mov eax, 569
	mov r11, rbp[-0x8]
	mov r11[0x8e8], eax

	mov eax, 570
	mov r11, rbp[-0x8]
	mov r11[0x8ec], eax

	mov eax, 571
	mov r11, rbp[-0x8]
	mov r11[0x8f0], eax

	mov eax, 572
	mov r11, rbp[-0x8]
	mov r11[0x8f4], eax

	mov eax, 573
	mov r11, rbp[-0x8]
	mov r11[0x8f8], eax

	mov eax, 574
	mov r11, rbp[-0x8]
	mov r11[0x8fc], eax

	mov eax, 575
	mov r11, rbp[-0x8]
	mov r11[0x900], eax

	mov eax, 576
	mov r11, rbp[-0x8]
	mov r11[0x904], eax

	mov eax, 577
	mov r11, rbp[-0x8]
	mov r11[0x908], eax

	mov eax, 578
	mov r11, rbp[-0x8]
	mov r11[0x90c], eax

	mov eax, 579
	mov r11, rbp[-0x8]
	mov r11[0x910], eax

	mov eax, 580
	mov r11, rbp[-0x8]
	mov r11[0x914], eax

	mov eax, 581
	mov r11, rbp[-0x8]
	mov r11[0x918], eax

	mov eax, 582
	mov r11, rbp[-0x8]
	mov r11[0x91c], eax

	mov eax, 583
	mov r11, rbp[-0x8]
	mov r11[0x920], eax

	mov eax, 584
	mov r11, rbp[-0x8]
	mov r11[0x924], eax

	mov eax, 585
	mov r11, rbp[-0x8]
	mov r11[0x928], eax

	mov eax, 586
	mov r11, rbp[-0x8]
	mov r11[0x92c], eax

	mov eax, 587
	mov r11, rbp[-0x8]
	mov r11[0x930], eax

	mov eax, 588
	mov r11, rbp[-0x8]
	mov r11[0x934], eax

	mov eax, 589
	mov r11, rbp[-0x8]
	mov r11[0x938], eax

	mov eax, 590
	mov r11, rbp[-0x8]
	mov r11[0x93c], eax

	mov eax, 591
	mov r11, rbp[-0x8]
	mov r11[0x940], eax

	mov eax, 592
	mov r11, rbp[-0x8]
	mov r11[0x944], eax

	mov eax, 593
	mov r11, rbp[-0x8]
	mov r11[0x948], eax

	mov eax, 594
	mov r11, rbp[-0x8]
	mov r11[0x94c], eax

	mov eax, 595
	mov r11, rbp[-0x8]
	mov r11[0x950], eax

	mov eax, 596
	mov r11, rbp[-0x8]
	mov r11[0x954], eax

	mov eax, 597
	mov r11, rbp[-0x8]
	mov r11[0x958], eax

	mov eax, 598
	mov r11, rbp[-0x8]
	mov r11[0x95c], eax

	mov eax, 599
	mov r11, rbp[-0x8]
	mov r11[0x960], eax

	mov eax, 600
	mov r11, rbp[-0x8]
	mov r11[0x964], eax

	mov eax, 601
	mov r11, rbp[-0x8]
	mov r11[0x968], eax

	mov eax, 602
	mov r11, rbp[-0x8]
	mov r11[0x96c], eax

	mov eax, 603
	mov r11, rbp[-0x8]
	mov r11[0x970], eax

	mov eax, 604
	mov r11, rbp[-0x8]
	mov r11[0x974], eax

	mov eax, 605
	mov r11, rbp[-0x8]
	mov r11[0x978], eax

	mov eax, 606
	mov r11, rbp[-0x8]
	mov r11[0x97c], eax

	mov eax, 607
	mov r11, rbp[-0x8]
	mov r11[0x980], eax

	mov eax, 608
	mov r11, rbp[-0x8]
	mov r11[0x984], eax

	mov eax, 609
	mov r11, rbp[-0x8]
	mov r11[0x988], eax

	mov eax, 610
	mov r11, rbp[-0x8]
	mov r11[0x98c], eax

	mov eax, 611
	mov r11, rbp[-0x8]
	mov r11[0x990], eax

	mov eax, 612
	mov r11, rbp[-0x8]
	mov r11[0x994], eax

	mov eax, 613
	mov r11, rbp[-0x8]
	mov r11[0x998], eax

	mov eax, 614
	mov r11, rbp[-0x8]
	mov r11[0x99c], eax

	mov eax, 615
	mov r11, rbp[-0x8]
	mov r11[0x9a0], eax

	mov eax, 616
	mov r11, rbp[-0x8]
	mov r11[0x9a4], eax

	mov eax, 617
	mov r11, rbp[-0x8]
	mov r11[0x9a8], eax

	mov eax, 618
	mov r11, rbp[-0x8]
	mov r11[0x9ac], eax

	mov eax, 619
	mov r11, rbp[-0x8]
	mov r11[0x9b0], eax

	mov eax, 620
	mov r11, rbp[-0x8]
	mov r11[0x9b4], eax

	mov eax, 621
	mov r11, rbp[-0x8]
	mov r11[0x9b8], eax

	mov eax, 622
	mov r11, rbp[-0x8]
	mov r11[0x9bc], eax

	mov eax, 623
	mov r11, rbp[-0x8]
	mov r11[0x9c0], eax

	mov eax, 624
	mov r11, rbp[-0x8]
	mov r11[0x9c4], eax

	mov eax, 625
	mov r11, rbp[-0x8]
	mov r11[0x9c8], eax

	mov eax, 626
	mov r11, rbp[-0x8]
	mov r11[0x9cc], eax

	mov eax, 627
	mov r11, rbp[-0x8]
	mov r11[0x9d0], eax

	mov eax, 628
	mov r11, rbp[-0x8]
	mov r11[0x9d4], eax

	mov eax, 629
	mov r11, rbp[-0x8]
	mov r11[0x9d8], eax

	mov eax, 630
	mov r11, rbp[-0x8]
	mov r11[0x9dc], eax

	mov eax, 631
	mov r11, rbp[-0x8]
	mov r11[0x9e0], eax

	mov eax, 632
	mov r11, rbp[-0x8]
	mov r11[0x9e4], eax

	mov eax, 633
	mov r11, rbp[-0x8]
	mov r11[0x9e8], eax

	mov eax, 634
	mov r11, rbp[-0x8]
	mov r11[0x9ec], eax

	mov eax, 635
	mov r11, rbp[-0x8]
	mov r11[0x9f0], eax

	mov eax, 636
	mov r11, rbp[-0x8]
	mov r11[0x9f4], eax

	mov eax, 637
	mov r11, rbp[-0x8]
	mov r11[0x9f8], eax

	mov eax, 638
	mov r11, rbp[-0x8]
	mov r11[0x9fc], eax

	mov eax, 639
	mov r11, rbp[-0x8]
	mov r11[0xa00], eax

	mov eax, 640
	mov r11, rbp[-0x8]
	mov r11[0xa04], eax

	mov eax, 641
	mov r11, rbp[-0x8]
	mov r11[0xa08], eax

	mov eax, 642
	mov r11, rbp[-0x8]
	mov r11[0xa0c], eax

	mov eax, 643
	mov r11, rbp[-0x8]
	mov r11[0xa10], eax

	mov eax, 644
	mov r11, rbp[-0x8]
	mov r11[0xa14], eax

	mov eax, 645
	mov r11, rbp[-0x8]
	mov r11[0xa18], eax

	mov eax, 646
	mov r11, rbp[-0x8]
	mov r11[0xa1c], eax

	mov eax, 647
	mov r11, rbp[-0x8]
	mov r11[0xa20], eax

	mov eax, 648
	mov r11, rbp[-0x8]
	mov r11[0xa24], eax

	mov eax, 649
	mov r11, rbp[-0x8]
	mov r11[0xa28], eax

	mov eax, 650
	mov r11, rbp[-0x8]
	mov r11[0xa2c], eax

	mov eax, 651
	mov r11, rbp[-0x8]
	mov r11[0xa30], eax

	mov eax, 652
	mov r11, rbp[-0x8]
	mov r11[0xa34], eax

	mov eax, 653
	mov r11, rbp[-0x8]
	mov r11[0xa38], eax

	mov eax, 654
	mov r11, rbp[-0x8]
	mov r11[0xa3c], eax

	mov eax, 655
	mov r11, rbp[-0x8]
	mov r11[0xa40], eax

	mov eax, 656
	mov r11, rbp[-0x8]
	mov r11[0xa44], eax

	mov eax, 657
	mov r11, rbp[-0x8]
	mov r11[0xa48], eax

	mov eax, 658
	mov r11, rbp[-0x8]
	mov r11[0xa4c], eax

	mov eax, 659
	mov r11, rbp[-0x8]
	mov r11[0xa50], eax

	mov eax, 660
	mov r11, rbp[-0x8]
	mov r11[0xa54], eax

	mov eax, 661
	mov r11, rbp[-0x8]
	mov r11[0xa58], eax

	mov eax, 662
	mov r11, rbp[-0x8]
	mov r11[0xa5c], eax

	mov eax, 663
	mov r11, rbp[-0x8]
	mov r11[0xa60], eax

	mov eax, 664
	mov r11, rbp[-0x8]
	mov r11[0xa64], eax

	mov eax, 665
	mov r11, rbp[-0x8]
	mov r11[0xa68], eax

	mov eax, 666
	mov r11, rbp[-0x8]
	mov r11[0xa6c], eax

	mov eax, 667
	mov r11, rbp[-0x8]
	mov r11[0xa70], eax

	mov eax, 668
	mov r11, rbp[-0x8]
	mov r11[0xa74], eax

	mov eax, 669
	mov r11, rbp[-0x8]
	mov r11[0xa78], eax

	mov eax, 670
	mov r11, rbp[-0x8]
	mov r11[0xa7c], eax

	mov eax, 671
	mov r11, rbp[-0x8]
	mov r11[0xa80], eax

	mov eax, 672
	mov r11, rbp[-0x8]
	mov r11[0xa84], eax

	mov eax, 673
	mov r11, rbp[-0x8]
	mov r11[0xa88], eax

	mov eax, 674
	mov r11, rbp[-0x8]
	mov r11[0xa8c], eax

	mov eax, 675
	mov r11, rbp[-0x8]
	mov r11[0xa90], eax

	mov eax, 676
	mov r11, rbp[-0x8]
	mov r11[0xa94], eax

	mov eax, 677
	mov r11, rbp[-0x8]
	mov r11[0xa98], eax

	mov eax, 678
	mov r11, rbp[-0x8]
	mov r11[0xa9c], eax

	mov eax, 679
	mov r11, rbp[-0x8]
	mov r11[0xaa0], eax

	mov eax, 680
	mov r11, rbp[-0x8]
	mov r11[0xaa4], eax

	mov eax, 681
	mov r11, rbp[-0x8]
	mov r11[0xaa8], eax

	mov eax, 682
	mov r11, rbp[-0x8]
	mov r11[0xaac], eax

	mov eax, 683
	mov r11, rbp[-0x8]
	mov r11[0xab0], eax

	mov eax, 684
	mov r11, rbp[-0x8]
	mov r11[0xab4], eax

	mov eax, 685
	mov r11, rbp[-0x8]
	mov r11[0xab8], eax

	mov eax, 686
	mov r11, rbp[-0x8]
	mov r11[0xabc], eax

	mov eax, 687
	mov r11, rbp[-0x8]
	mov r11[0xac0], eax

	mov eax, 688
	mov r11, rbp[-0x8]
	mov r11[0xac4], eax

	mov eax, 689
	mov r11, rbp[-0x8]
	mov r11[0xac8], eax

	mov eax, 690
	mov r11, rbp[-0x8]
	mov r11[0xacc], eax

	mov eax, 691
	mov r11, rbp[-0x8]
	mov r11[0xad0], eax

	mov eax, 692
	mov r11, rbp[-0x8]
	mov r11[0xad4], eax

	mov eax, 693
	mov r11, rbp[-0x8]
	mov r11[0xad8], eax

	mov eax, 694
	mov r11, rbp[-0x8]
	mov r11[0xadc], eax

	mov eax, 695
	mov r11, rbp[-0x8]
	mov r11[0xae0], eax

	mov eax, 696
	mov r11, rbp[-0x8]
	mov r11[0xae4], eax

	mov eax, 697
	mov r11, rbp[-0x8]
	mov r11[0xae8], eax

	mov eax, 698
	mov r11, rbp[-0x8]
	mov r11[0xaec], eax

	mov eax, 699
	mov r11, rbp[-0x8]
	mov r11[0xaf0], eax

	mov eax, 700
	mov r11, rbp[-0x8]
	mov r11[0xaf4], eax

	mov eax, 701
	mov r11, rbp[-0x8]
	mov r11[0xaf8], eax

	mov eax, 702
	mov r11, rbp[-0x8]
	mov r11[0xafc], eax

	mov eax, 703
	mov r11, rbp[-0x8]
	mov r11[0xb00], eax

	mov eax, 704
	mov r11, rbp[-0x8]
	mov r11[0xb04], eax

	mov eax, 705
	mov r11, rbp[-0x8]
	mov r11[0xb08], eax

	mov eax, 706
	mov r11, rbp[-0x8]
	mov r11[0xb0c], eax

	mov eax, 707
	mov r11, rbp[-0x8]
	mov r11[0xb10], eax

	mov eax, 708
	mov r11, rbp[-0x8]
	mov r11[0xb14], eax

	mov eax, 709
	mov r11, rbp[-0x8]
	mov r11[0xb18], eax

	mov eax, 710
	mov r11, rbp[-0x8]
	mov r11[0xb1c], eax

	mov eax, 711
	mov r11, rbp[-0x8]
	mov r11[0xb20], eax

	mov eax, 712
	mov r11, rbp[-0x8]
	mov r11[0xb24], eax

	mov eax, 713
	mov r11, rbp[-0x8]
	mov r11[0xb28], eax

	mov eax, 714
	mov r11, rbp[-0x8]
	mov r11[0xb2c], eax

	mov eax, 715
	mov r11, rbp[-0x8]
	mov r11[0xb30], eax

	mov eax, 716
	mov r11, rbp[-0x8]
	mov r11[0xb34], eax

	mov eax, 717
	mov r11, rbp[-0x8]
	mov r11[0xb38], eax

	mov eax, 718
	mov r11, rbp[-0x8]
	mov r11[0xb3c], eax

	mov eax, 719
	mov r11, rbp[-0x8]
	mov r11[0xb40], eax

	mov eax, 720
	mov r11, rbp[-0x8]
	mov r11[0xb44], eax

	mov eax, 721
	mov r11, rbp[-0x8]
	mov r11[0xb48], eax

	mov eax, 722
	mov r11, rbp[-0x8]
	mov r11[0xb4c], eax

	mov eax, 723
	mov r11, rbp[-0x8]
	mov r11[0xb50], eax

	mov eax, 724
	mov r11, rbp[-0x8]
	mov r11[0xb54], eax

	mov eax, 725
	mov r11, rbp[-0x8]
	mov r11[0xb58], eax

	mov eax, 726
	mov r11, rbp[-0x8]
	mov r11[0xb5c], eax

	mov eax, 727
	mov r11, rbp[-0x8]
	mov r11[0xb60], eax

	mov eax, 728
	mov r11, rbp[-0x8]
	mov r11[0xb64], eax

	mov eax, 729
	mov r11, rbp[-0x8]
	mov r11[0xb68], eax

	mov eax, 730
	mov r11, rbp[-0x8]
	mov r11[0xb6c], eax

	mov eax, 731
	mov r11, rbp[-0x8]
	mov r11[0xb70], eax

	mov eax, 732
	mov r11, rbp[-0x8]
	mov r11[0xb74], eax

	mov eax, 733
	mov r11, rbp[-0x8]
	mov r11[0xb78], eax

	mov eax, 734
	mov r11, rbp[-0x8]
	mov r11[0xb7c], eax

	mov eax, 735
	mov r11, rbp[-0x8]
	mov r11[0xb80], eax

	mov eax, 736
	mov r11, rbp[-0x8]
	mov r11[0xb84], eax

	mov eax, 737
	mov r11, rbp[-0x8]
	mov r11[0xb88], eax

	mov eax, 738
	mov r11, rbp[-0x8]
	mov r11[0xb8c], eax

	mov eax, 739
	mov r11, rbp[-0x8]
	mov r11[0xb90], eax

	mov eax, 740
	mov r11, rbp[-0x8]
	mov r11[0xb94], eax

	mov eax, 741
	mov r11, rbp[-0x8]
	mov r11[0xb98], eax

	mov eax, 742
	mov r11, rbp[-0x8]
	mov r11[0xb9c], eax

	mov eax, 743
	mov r11, rbp[-0x8]
	mov r11[0xba0], eax

	mov eax, 744
	mov r11, rbp[-0x8]
	mov r11[0xba4], eax

	mov eax, 745
	mov r11, rbp[-0x8]
	mov r11[0xba8], eax

	mov eax, 746
	mov r11, rbp[-0x8]
	mov r11[0xbac], eax

	mov eax, 747
	mov r11, rbp[-0x8]
	mov r11[0xbb0], eax

	mov eax, 748
	mov r11, rbp[-0x8]
	mov r11[0xbb4], eax

	mov eax, 749
	mov r11, rbp[-0x8]
	mov r11[0xbb8], eax

	mov eax, 750
	mov r11, rbp[-0x8]
	mov r11[0xbbc], eax

	mov eax, 751
	mov r11, rbp[-0x8]
	mov r11[0xbc0], eax

	mov eax, 752
	mov r11, rbp[-0x8]
	mov r11[0xbc4], eax

	mov eax, 753
	mov r11, rbp[-0x8]
	mov r11[0xbc8], eax

	mov eax, 754
	mov r11, rbp[-0x8]
	mov r11[0xbcc], eax

	mov eax, 755
	mov r11, rbp[-0x8]
	mov r11[0xbd0], eax

	mov eax, 756
	mov r11, rbp[-0x8]
	mov r11[0xbd4], eax

	mov eax, 757
	mov r11, rbp[-0x8]
	mov r11[0xbd8], eax

	mov eax, 758
	mov r11, rbp[-0x8]
	mov r11[0xbdc], eax

	mov eax, 759
	mov r11, rbp[-0x8]
	mov r11[0xbe0], eax

	mov eax, 760
	mov r11, rbp[-0x8]
	mov r11[0xbe4], eax

	mov eax, 761
	mov r11, rbp[-0x8]
	mov r11[0xbe8], eax

	mov eax, 762
	mov r11, rbp[-0x8]
	mov r11[0xbec], eax

	mov eax, 763
	mov r11, rbp[-0x8]
	mov r11[0xbf0], eax

	mov eax, 764
	mov r11, rbp[-0x8]
	mov r11[0xbf4], eax

	mov eax, 765
	mov r11, rbp[-0x8]
	mov r11[0xbf8], eax

	mov eax, 766
	mov r11, rbp[-0x8]
	mov r11[0xbfc], eax

	mov eax, 767
	mov r11, rbp[-0x8]
	mov r11[0xc00], eax

	mov eax, 768
	mov r11, rbp[-0x8]
	mov r11[0xc04], eax

	mov eax, 769
	mov r11, rbp[-0x8]
	mov r11[0xc08], eax

	mov eax, 770
	mov r11, rbp[-0x8]
	mov r11[0xc0c], eax

	mov eax, 771
	mov r11, rbp[-0x8]
	mov r11[0xc10], eax

	mov eax, 772
	mov r11, rbp[-0x8]
	mov r11[0xc14], eax

	mov eax, 773
	mov r11, rbp[-0x8]
	mov r11[0xc18], eax

	mov eax, 774
	mov r11, rbp[-0x8]
	mov r11[0xc1c], eax

	mov eax, 775
	mov r11, rbp[-0x8]
	mov r11[0xc20], eax

	mov eax, 776
	mov r11, rbp[-0x8]
	mov r11[0xc24], eax

	mov eax, 777
	mov r11, rbp[-0x8]
	mov r11[0xc28], eax

	mov eax, 778
	mov r11, rbp[-0x8]
	mov r11[0xc2c], eax

	mov eax, 779
	mov r11, rbp[-0x8]
	mov r11[0xc30], eax

	mov eax, 780
	mov r11, rbp[-0x8]
	mov r11[0xc34], eax

	mov eax, 781
	mov r11, rbp[-0x8]
	mov r11[0xc38], eax

	mov eax, 782
	mov r11, rbp[-0x8]
	mov r11[0xc3c], eax

	mov eax, 783
	mov r11, rbp[-0x8]
	mov r11[0xc40], eax

	mov eax, 784
	mov r11, rbp[-0x8]
	mov r11[0xc44], eax

	mov eax, 785
	mov r11, rbp[-0x8]
	mov r11[0xc48], eax

	mov eax, 786
	mov r11, rbp[-0x8]
	mov r11[0xc4c], eax

	mov eax, 787
	mov r11, rbp[-0x8]
	mov r11[0xc50], eax

	mov eax, 788
	mov r11, rbp[-0x8]
	mov r11[0xc54], eax

	mov eax, 789
	mov r11, rbp[-0x8]
	mov r11[0xc58], eax

	mov eax, 790
	mov r11, rbp[-0x8]
	mov r11[0xc5c], eax

	mov eax, 791
	mov r11, rbp[-0x8]
	mov r11[0xc60], eax

	mov eax, 792
	mov r11, rbp[-0x8]
	mov r11[0xc64], eax

	mov eax, 793
	mov r11, rbp[-0x8]
	mov r11[0xc68], eax

	mov eax, 794
	mov r11, rbp[-0x8]
	mov r11[0xc6c], eax

	mov eax, 795
	mov r11, rbp[-0x8]
	mov r11[0xc70], eax

	mov eax, 796
	mov r11, rbp[-0x8]
	mov r11[0xc74], eax

	mov eax, 797
	mov r11, rbp[-0x8]
	mov r11[0xc78], eax

	mov eax, 798
	mov r11, rbp[-0x8]
	mov r11[0xc7c], eax

	mov eax, 799
	mov r11, rbp[-0x8]
	mov r11[0xc80], eax

	mov eax, 800
	mov r11, rbp[-0x8]
	mov r11[0xc84], eax

	mov eax, 801
	mov r11, rbp[-0x8]
	mov r11[0xc88], eax

	mov eax, 802
	mov r11, rbp[-0x8]
	mov r11[0xc8c], eax

	mov eax, 803
	mov r11, rbp[-0x8]
	mov r11[0xc90], eax

	mov eax, 804
	mov r11, rbp[-0x8]
	mov r11[0xc94], eax

	mov eax, 805
	mov r11, rbp[-0x8]
	mov r11[0xc98], eax

	mov eax, 806
	mov r11, rbp[-0x8]
	mov r11[0xc9c], eax

	mov eax, 807
	mov r11, rbp[-0x8]
	mov r11[0xca0], eax

	mov eax, 808
	mov r11, rbp[-0x8]
	mov r11[0xca4], eax

	mov eax, 809
	mov r11, rbp[-0x8]
	mov r11[0xca8], eax

	mov eax, 810
	mov r11, rbp[-0x8]
	mov r11[0xcac], eax

	mov eax, 811
	mov r11, rbp[-0x8]
	mov r11[0xcb0], eax

	mov eax, 812
	mov r11, rbp[-0x8]
	mov r11[0xcb4], eax

	mov eax, 813
	mov r11, rbp[-0x8]
	mov r11[0xcb8], eax

	mov eax, 814
	mov r11, rbp[-0x8]
	mov r11[0xcbc], eax

	mov eax, 815
	mov r11, rbp[-0x8]
	mov r11[0xcc0], eax

	mov eax, 816
	mov r11, rbp[-0x8]
	mov r11[0xcc4], eax

	mov eax, 817
	mov r11, rbp[-0x8]
	mov r11[0xcc8], eax

	mov eax, 818
	mov r11, rbp[-0x8]
	mov r11[0xccc], eax

	mov eax, 819
	mov r11, rbp[-0x8]
	mov r11[0xcd0], eax

	mov eax, 820
	mov r11, rbp[-0x8]
	mov r11[0xcd4], eax

	mov eax, 821
	mov r11, rbp[-0x8]
	mov r11[0xcd8], eax

	mov eax, 822
	mov r11, rbp[-0x8]
	mov r11[0xcdc], eax

	mov eax, 823
	mov r11, rbp[-0x8]
	mov r11[0xce0], eax

	mov eax, 824
	mov r11, rbp[-0x8]
	mov r11[0xce4], eax

	mov eax, 825
	mov r11, rbp[-0x8]
	mov r11[0xce8], eax

	mov eax, 826
	mov r11, rbp[-0x8]
	mov r11[0xcec], eax

	mov eax, 827
	mov r11, rbp[-0x8]
	mov r11[0xcf0], eax

	mov eax, 828
	mov r11, rbp[-0x8]
	mov r11[0xcf4], eax

	mov eax, 829
	mov r11, rbp[-0x8]
	mov r11[0xcf8], eax

	mov eax, 830
	mov r11, rbp[-0x8]
	mov r11[0xcfc], eax

	mov eax, 831
	mov r11, rbp[-0x8]
	mov r11[0xd00], eax

	mov eax, 832
	mov r11, rbp[-0x8]
	mov r11[0xd04], eax

	mov eax, 833
	mov r11, rbp[-0x8]
	mov r11[0xd08], eax

	mov eax, 834
	mov r11, rbp[-0x8]
	mov r11[0xd0c], eax

	mov eax, 835
	mov r11, rbp[-0x8]
	mov r11[0xd10], eax

	mov eax, 836
	mov r11, rbp[-0x8]
	mov r11[0xd14], eax

	mov eax, 837
	mov r11, rbp[-0x8]
	mov r11[0xd18], eax

	mov eax, 838
	mov r11, rbp[-0x8]
	mov r11[0xd1c], eax

	mov eax, 839
	mov r11, rbp[-0x8]
	mov r11[0xd20], eax

	mov eax, 840
	mov r11, rbp[-0x8]
	mov r11[0xd24], eax

	mov eax, 841
	mov r11, rbp[-0x8]
	mov r11[0xd28], eax

	mov eax, 842
	mov r11, rbp[-0x8]
	mov r11[0xd2c], eax

	mov eax, 843
	mov r11, rbp[-0x8]
	mov r11[0xd30], eax

	mov eax, 844
	mov r11, rbp[-0x8]
	mov r11[0xd34], eax

	mov eax, 845
	mov r11, rbp[-0x8]
	mov r11[0xd38], eax

	mov eax, 846
	mov r11, rbp[-0x8]
	mov r11[0xd3c], eax

	mov eax, 847
	mov r11, rbp[-0x8]
	mov r11[0xd40], eax

	mov eax, 848
	mov r11, rbp[-0x8]
	mov r11[0xd44], eax

	mov eax, 849
	mov r11, rbp[-0x8]
	mov r11[0xd48], eax

	mov eax, 850
	mov r11, rbp[-0x8]
	mov r11[0xd4c], eax

	mov eax, 851
	mov r11, rbp[-0x8]
	mov r11[0xd50], eax

	mov eax, 852
	mov r11, rbp[-0x8]
	mov r11[0xd54], eax

	mov eax, 853
	mov r11, rbp[-0x8]
	mov r11[0xd58], eax

	mov eax, 854
	mov r11, rbp[-0x8]
	mov r11[0xd5c], eax

	mov eax, 855
	mov r11, rbp[-0x8]
	mov r11[0xd60], eax

	mov eax, 856
	mov r11, rbp[-0x8]
	mov r11[0xd64], eax

	mov eax, 857
	mov r11, rbp[-0x8]
	mov r11[0xd68], eax

	mov eax, 858
	mov r11, rbp[-0x8]
	mov r11[0xd6c], eax

	mov eax, 859
	mov r11, rbp[-0x8]
	mov r11[0xd70], eax

	mov eax, 860
	mov r11, rbp[-0x8]
	mov r11[0xd74], eax

	mov eax, 861
	mov r11, rbp[-0x8]
	mov r11[0xd78], eax

	mov eax, 862
	mov r11, rbp[-0x8]
	mov r11[0xd7c], eax

	mov eax, 863
	mov r11, rbp[-0x8]
	mov r11[0xd80], eax

	mov eax, 864
	mov r11, rbp[-0x8]
	mov r11[0xd84], eax

	mov eax, 865
	mov r11, rbp[-0x8]
	mov r11[0xd88], eax

	mov eax, 866
	mov r11, rbp[-0x8]
	mov r11[0xd8c], eax

	mov eax, 867
	mov r11, rbp[-0x8]
	mov r11[0xd90], eax

	mov eax, 868
	mov r11, rbp[-0x8]
	mov r11[0xd94], eax

	mov eax, 869
	mov r11, rbp[-0x8]
	mov r11[0xd98], eax

	mov eax, 870
	mov r11, rbp[-0x8]
	mov r11[0xd9c], eax

	mov eax, 871
	mov r11, rbp[-0x8]
	mov r11[0xda0], eax

	mov eax, 872
	mov r11, rbp[-0x8]
	mov r11[0xda4], eax

	mov eax, 873
	mov r11, rbp[-0x8]
	mov r11[0xda8], eax

	mov eax, 874
	mov r11, rbp[-0x8]
	mov r11[0xdac], eax

	mov eax, 875
	mov r11, rbp[-0x8]
	mov r11[0xdb0], eax

	mov eax, 876
	mov r11, rbp[-0x8]
	mov r11[0xdb4], eax

	mov eax, 877
	mov r11, rbp[-0x8]
	mov r11[0xdb8], eax

	mov eax, 878
	mov r11, rbp[-0x8]
	mov r11[0xdbc], eax

	mov eax, 879
	mov r11, rbp[-0x8]
	mov r11[0xdc0], eax

	mov eax, 880
	mov r11, rbp[-0x8]
	mov r11[0xdc4], eax

	mov eax, 881
	mov r11, rbp[-0x8]
	mov r11[0xdc8], eax

	mov eax, 882
	mov r11, rbp[-0x8]
	mov r11[0xdcc], eax

	mov eax, 883
	mov r11, rbp[-0x8]
	mov r11[0xdd0], eax

	mov eax, 884
	mov r11, rbp[-0x8]
	mov r11[0xdd4], eax

	mov eax, 885
	mov r11, rbp[-0x8]
	mov r11[0xdd8], eax

	mov eax, 886
	mov r11, rbp[-0x8]
	mov r11[0xddc], eax

	mov eax, 887
	mov r11, rbp[-0x8]
	mov r11[0xde0], eax

	mov eax, 888
	mov r11, rbp[-0x8]
	mov r11[0xde4], eax

	mov eax, 889
	mov r11, rbp[-0x8]
	mov r11[0xde8], eax

	mov eax, 890
	mov r11, rbp[-0x8]
	mov r11[0xdec], eax

	mov eax, 891
	mov r11, rbp[-0x8]
	mov r11[0xdf0], eax

	mov eax, 892
	mov r11, rbp[-0x8]
	mov r11[0xdf4], eax

	mov eax, 893
	mov r11, rbp[-0x8]
	mov r11[0xdf8], eax

	mov eax, 894
	mov r11, rbp[-0x8]
	mov r11[0xdfc], eax

	mov eax, 895
	mov r11, rbp[-0x8]
	mov r11[0xe00], eax

	mov eax, 896
	mov r11, rbp[-0x8]
	mov r11[0xe04], eax

	mov eax, 897
	mov r11, rbp[-0x8]
	mov r11[0xe08], eax

	mov eax, 898
	mov r11, rbp[-0x8]
	mov r11[0xe0c], eax

	mov eax, 899
	mov r11, rbp[-0x8]
	mov r11[0xe10], eax

	mov eax, 900
	mov r11, rbp[-0x8]
	mov r11[0xe14], eax

	mov eax, 901
	mov r11, rbp[-0x8]
	mov r11[0xe18], eax

	mov eax, 902
	mov r11, rbp[-0x8]
	mov r11[0xe1c], eax

	mov eax, 903
	mov r11, rbp[-0x8]
	mov r11[0xe20], eax

	mov eax, 904
	mov r11, rbp[-0x8]
	mov r11[0xe24], eax

	mov eax, 905
	mov r11, rbp[-0x8]
	mov r11[0xe28], eax

	mov eax, 906
	mov r11, rbp[-0x8]
	mov r11[0xe2c], eax

	mov eax, 907
	mov r11, rbp[-0x8]
	mov r11[0xe30], eax

	mov eax, 908
	mov r11, rbp[-0x8]
	mov r11[0xe34], eax

	mov eax, 909
	mov r11, rbp[-0x8]
	mov r11[0xe38], eax

	mov eax, 910
	mov r11, rbp[-0x8]
	mov r11[0xe3c], eax

	mov eax, 911
	mov r11, rbp[-0x8]
	mov r11[0xe40], eax

	mov eax, 912
	mov r11, rbp[-0x8]
	mov r11[0xe44], eax

	mov eax, 913
	mov r11, rbp[-0x8]
	mov r11[0xe48], eax

	mov eax, 914
	mov r11, rbp[-0x8]
	mov r11[0xe4c], eax

	mov eax, 915
	mov r11, rbp[-0x8]
	mov r11[0xe50], eax

	mov eax, 916
	mov r11, rbp[-0x8]
	mov r11[0xe54], eax

	mov eax, 917
	mov r11, rbp[-0x8]
	mov r11[0xe58], eax

	mov eax, 918
	mov r11, rbp[-0x8]
	mov r11[0xe5c], eax

	mov eax, 919
	mov r11, rbp[-0x8]
	mov r11[0xe60], eax

	mov eax, 920
	mov r11, rbp[-0x8]
	mov r11[0xe64], eax

	mov eax, 921
	mov r11, rbp[-0x8]
	mov r11[0xe68], eax

	mov eax, 922
	mov r11, rbp[-0x8]
	mov r11[0xe6c], eax

	mov eax, 923
	mov r11, rbp[-0x8]
	mov r11[0xe70], eax

	mov eax, 924
	mov r11, rbp[-0x8]
	mov r11[0xe74], eax

	mov eax, 925
	mov r11, rbp[-0x8]
	mov r11[0xe78], eax

	mov eax, 926
	mov r11, rbp[-0x8]
	mov r11[0xe7c], eax

	mov eax, 927
	mov r11, rbp[-0x8]
	mov r11[0xe80], eax

	mov eax, 928
	mov r11, rbp[-0x8]
	mov r11[0xe84], eax

	mov eax, 929
	mov r11, rbp[-0x8]
	mov r11[0xe88], eax

	mov eax, 930
	mov r11, rbp[-0x8]
	mov r11[0xe8c], eax

	mov eax, 931
	mov r11, rbp[-0x8]
	mov r11[0xe90], eax

	mov eax, 932
	mov r11, rbp[-0x8]
	mov r11[0xe94], eax

	mov eax, 933
	mov r11, rbp[-0x8]
	mov r11[0xe98], eax

	mov eax, 934
	mov r11, rbp[-0x8]
	mov r11[0xe9c], eax

	mov eax, 935
	mov r11, rbp[-0x8]
	mov r11[0xea0], eax

	mov eax, 936
	mov r11, rbp[-0x8]
	mov r11[0xea4], eax

	mov eax, 937
	mov r11, rbp[-0x8]
	mov r11[0xea8], eax

	mov eax, 938
	mov r11, rbp[-0x8]
	mov r11[0xeac], eax

	mov eax, 939
	mov r11, rbp[-0x8]
	mov r11[0xeb0], eax

	mov eax, 940
	mov r11, rbp[-0x8]
	mov r11[0xeb4], eax

	mov eax, 941
	mov r11, rbp[-0x8]
	mov r11[0xeb8], eax

	mov eax, 942
	mov r11, rbp[-0x8]
	mov r11[0xebc], eax

	mov eax, 943
	mov r11, rbp[-0x8]
	mov r11[0xec0], eax

	mov eax, 944
	mov r11, rbp[-0x8]
	mov r11[0xec4], eax

	mov eax, 945
	mov r11, rbp[-0x8]
	mov r11[0xec8], eax

	mov eax, 946
	mov r11, rbp[-0x8]
	mov r11[0xecc], eax

	mov eax, 947
	mov r11, rbp[-0x8]
	mov r11[0xed0], eax

	mov eax, 948
	mov r11, rbp[-0x8]
	mov r11[0xed4], eax

	mov eax, 949
	mov r11, rbp[-0x8]
	mov r11[0xed8], eax

	mov eax, 950
	mov r11, rbp[-0x8]
	mov r11[0xedc], eax

	mov eax, 951
	mov r11, rbp[-0x8]
	mov r11[0xee0], eax

	mov eax, 952
	mov r11, rbp[-0x8]
	mov r11[0xee4], eax

	mov eax, 953
	mov r11, rbp[-0x8]
	mov r11[0xee8], eax

	mov eax, 954
	mov r11, rbp[-0x8]
	mov r11[0xeec], eax

	mov eax, 955
	mov r11, rbp[-0x8]
	mov r11[0xef0], eax

	mov eax, 956
	mov r11, rbp[-0x8]
	mov r11[0xef4], eax

	mov eax, 957
	mov r11, rbp[-0x8]
	mov r11[0xef8], eax

	mov eax, 958
	mov r11, rbp[-0x8]
	mov r11[0xefc], eax

	mov eax, 959
	mov r11, rbp[-0x8]
	mov r11[0xf00], eax

	mov eax, 960
	mov r11, rbp[-0x8]
	mov r11[0xf04], eax

	mov eax, 961
	mov r11, rbp[-0x8]
	mov r11[0xf08], eax

	mov eax, 962
	mov r11, rbp[-0x8]
	mov r11[0xf0c], eax

	mov eax, 963
	mov r11, rbp[-0x8]
	mov r11[0xf10], eax

	mov eax, 964
	mov r11, rbp[-0x8]
	mov r11[0xf14], eax

	mov eax, 965
	mov r11, rbp[-0x8]
	mov r11[0xf18], eax

	mov eax, 966
	mov r11, rbp[-0x8]
	mov r11[0xf1c], eax

	mov eax, 967
	mov r11, rbp[-0x8]
	mov r11[0xf20], eax

	mov eax, 968
	mov r11, rbp[-0x8]
	mov r11[0xf24], eax

	mov eax, 969
	mov r11, rbp[-0x8]
	mov r11[0xf28], eax

	mov eax, 970
	mov r11, rbp[-0x8]
	mov r11[0xf2c], eax

	mov eax, 971
	mov r11, rbp[-0x8]
	mov r11[0xf30], eax

	mov eax, 972
	mov r11, rbp[-0x8]
	mov r11[0xf34], eax

	mov eax, 973
	mov r11, rbp[-0x8]
	mov r11[0xf38], eax

	mov eax, 974
	mov r11, rbp[-0x8]
	mov r11[0xf3c], eax

	mov eax, 975
	mov r11, rbp[-0x8]
	mov r11[0xf40], eax

	mov eax, 976
	mov r11, rbp[-0x8]
	mov r11[0xf44], eax

	mov eax, 977
	mov r11, rbp[-0x8]
	mov r11[0xf48], eax

	mov eax, 978
	mov r11, rbp[-0x8]
	mov r11[0xf4c], eax

	mov eax, 979
	mov r11, rbp[-0x8]
	mov r11[0xf50], eax

	mov eax, 980
	mov r11, rbp[-0x8]
	mov r11[0xf54], eax

	mov eax, 981
	mov r11, rbp[-0x8]
	mov r11[0xf58], eax

	mov eax, 982
	mov r11, rbp[-0x8]
	mov r11[0xf5c], eax

	mov eax, 983
	mov r11, rbp[-0x8]
	mov r11[0xf60], eax

	mov eax, 984
	mov r11, rbp[-0x8]
	mov r11[0xf64], eax

	mov eax, 985
	mov r11, rbp[-0x8]
	mov r11[0xf68], eax

	mov eax, 986
	mov r11, rbp[-0x8]
	mov r11[0xf6c], eax

	mov eax, 987
	mov r11, rbp[-0x8]
	mov r11[0xf70], eax

	mov eax, 988
	mov r11, rbp[-0x8]
	mov r11[0xf74], eax

	mov eax, 989
	mov r11, rbp[-0x8]
	mov r11[0xf78], eax

	mov eax, 990
	mov r11, rbp[-0x8]
	mov r11[0xf7c], eax

	mov eax, 991
	mov r11, rbp[-0x8]
	mov r11[0xf80], eax

	mov eax, 992
	mov r11, rbp[-0x8]
	mov r11[0xf84], eax

	mov eax, 993
	mov r11, rbp[-0x8]
	mov r11[0xf88], eax

	mov eax, 994
	mov r11, rbp[-0x8]
	mov r11[0xf8c], eax

	mov eax, 995
	mov r11, rbp[-0x8]
	mov r11[0xf90], eax

	mov eax, 996
	mov r11, rbp[-0x8]
	mov r11[0xf94], eax

	mov eax, 997
	mov r11, rbp[-0x8]
	mov r11[0xf98], eax

	mov eax, 998
	mov r11, rbp[-0x8]
	mov r11[0xf9c], eax

	mov eax, 999
	mov r11, rbp[-0x8]
	mov r11[0xfa0], eax

	mov eax, 1000
	mov r11, rbp[-0x8]
	mov r11[0xfa4], eax

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov eax, 1
	mov r11, rbp[-0x8]
	mov r11[byte 0x8], eax

	mov eax, 2
	mov r11, rbp[-0x8]
	mov r11[byte 0xc], eax

	mov eax, 3
	mov r11, rbp[-0x8]
	mov r11[byte 0x10], eax

	mov eax, 4
	mov r11, rbp[-0x8]
	mov r11[byte 0x14], eax

	mov eax, 5
	mov r11, rbp[-0x8]
	mov r11[byte 0x18], eax

	mov eax, 6
	mov r11, rbp[-0x8]
	mov r11[byte 0x1c], eax

	mov eax, 7
	mov r11, rbp[-0x8]
	mov r11[byte 0x20], eax

	mov eax, 8
	mov r11, rbp[-0x8]
	mov r11[byte 0x24], eax

	mov eax, 9
	mov r11, rbp[-0x8]
	mov r11[byte 0x28], eax

	mov eax, 10
	mov r11, rbp[-0x8]
	mov r11[byte 0x2c], eax

	mov eax, 11
	mov r11, rbp[-0x8]
	mov r11[byte 0x30], eax

	mov eax, 12
	mov r11, rbp[-0x8]
	mov r11[byte 0x34], eax

	mov eax, 13
	mov r11, rbp[-0x8]
	mov r11[byte 0x38], eax

	mov eax, 14
	mov r11, rbp[-0x8]
	mov r11[byte 0x3c], eax

	mov eax, 15
	mov r11, rbp[-0x8]
	mov r11[byte 0x40], eax

	mov eax, 16
	mov r11, rbp[-0x8]
	mov r11[byte 0x44], eax

	mov eax, 17
	mov r11, rbp[-0x8]
	mov r11[byte 0x48], eax

	mov eax, 18
	mov r11, rbp[-0x8]
	mov r11[byte 0x4c], eax

	mov eax, 19
	mov r11, rbp[-0x8]
	mov r11[byte 0x50], eax

	mov eax, 20
	mov r11, rbp[-0x8]
	mov r11[byte 0x54], eax

	mov eax, 21
	mov r11, rbp[-0x8]
	mov r11[byte 0x58], eax

	mov eax, 22
	mov r11, rbp[-0x8]
	mov r11[byte 0x5c], eax

	mov eax, 23
	mov r11, rbp[-0x8]
	mov r11[byte 0x60], eax

	mov eax, 24
	mov r11, rbp[-0x8]
	mov r11[byte 0x64], eax

	mov eax, 25
	mov r11, rbp[-0x8]
	mov r11[byte 0x68], eax

	mov eax, 26
	mov r11, rbp[-0x8]
	mov r11[byte 0x6c], eax

	mov eax, 27
	mov r11, rbp[-0x8]
	mov r11[byte 0x70], eax

	mov eax, 28
	mov r11, rbp[-0x8]
	mov r11[byte 0x74], eax

	mov eax, 29
	mov r11, rbp[-0x8]
	mov r11[byte 0x78], eax

	mov eax, 30
	mov r11, rbp[-0x8]
	mov r11[byte 0x7c], eax

	mov eax, 31
	mov r11, rbp[-0x8]
	mov r11[0x80], eax

	mov eax, 32
	mov r11, rbp[-0x8]
	mov r11[0x84], eax

	mov eax, 33
	mov r11, rbp[-0x8]
	mov r11[0x88], eax

	mov eax, 34
	mov r11, rbp[-0x8]
	mov r11[0x8c], eax

	mov eax, 35
	mov r11, rbp[-0x8]
	mov r11[0x90], eax

	mov eax, 36
	mov r11, rbp[-0x8]
	mov r11[0x94], eax

	mov eax, 37
	mov r11, rbp[-0x8]
	mov r11[0x98], eax

	mov eax, 38
	mov r11, rbp[-0x8]
	mov r11[0x9c], eax

	mov eax, 39
	mov r11, rbp[-0x8]
	mov r11[0xa0], eax

	mov eax, 40
	mov r11, rbp[-0x8]
	mov r11[0xa4], eax

	mov eax, 41
	mov r11, rbp[-0x8]
	mov r11[0xa8], eax

	mov eax, 42
	mov r11, rbp[-0x8]
	mov r11[0xac], eax

	mov eax, 43
	mov r11, rbp[-0x8]
	mov r11[0xb0], eax

	mov eax, 44
	mov r11, rbp[-0x8]
	mov r11[0xb4], eax

	mov eax, 45
	mov r11, rbp[-0x8]
	mov r11[0xb8], eax

	mov eax, 46
	mov r11, rbp[-0x8]
	mov r11[0xbc], eax

	mov eax, 47
	mov r11, rbp[-0x8]
	mov r11[0xc0], eax

	mov eax, 48
	mov r11, rbp[-0x8]
	mov r11[0xc4], eax

	mov eax, 49
	mov r11, rbp[-0x8]
	mov r11[0xc8], eax

	mov eax, 50
	mov r11, rbp[-0x8]
	mov r11[0xcc], eax

	mov eax, 51
	mov r11, rbp[-0x8]
	mov r11[0xd0], eax

	mov eax, 52
	mov r11, rbp[-0x8]
	mov r11[0xd4], eax

	mov eax, 53
	mov r11, rbp[-0x8]
	mov r11[0xd8], eax

	mov eax, 54
	mov r11, rbp[-0x8]
	mov r11[0xdc], eax

	mov eax, 55
	mov r11, rbp[-0x8]
	mov r11[0xe0], eax

	mov eax, 56
	mov r11, rbp[-0x8]
	mov r11[0xe4], eax

	mov eax, 57
	mov r11, rbp[-0x8]
	mov r11[0xe8], eax

	mov eax, 58
	mov r11, rbp[-0x8]
	mov r11[0xec], eax

	mov eax, 59
	mov r11, rbp[-0x8]
	mov r11[0xf0], eax

	mov eax, 60
	mov r11, rbp[-0x8]
	mov r11[0xf4], eax

	mov eax, 61
	mov r11, rbp[-0x8]
	mov r11[0xf8], eax

	mov eax, 62
	mov r11, rbp[-0x8]
	mov r11[0xfc], eax

	mov eax, 63
	mov r11, rbp[-0x8]
	mov r11[0x100], eax

	mov eax, 64
	mov r11, rbp[-0x8]
	mov r11[0x104], eax

	mov eax, 65
	mov r11, rbp[-0x8]
	mov r11[0x108], eax

	mov eax, 66
	mov r11, rbp[-0x8]
	mov r11[0x10c], eax

	mov eax, 67
	mov r11, rbp[-0x8]
	mov r11[0x110], eax

	mov eax, 68
	mov r11, rbp[-0x8]
	mov r11[0x114], eax

	mov eax, 69
	mov r11, rbp[-0x8]
	mov r11[0x118], eax

	mov eax, 70
	mov r11, rbp[-0x8]
	mov r11[0x11c], eax

	mov eax, 71
	mov r11, rbp[-0x8]
	mov r11[0x120], eax

	mov eax, 72
	mov r11, rbp[-0x8]
	mov r11[0x124], eax

	mov eax, 73
	mov r11, rbp[-0x8]
	mov r11[0x128], eax

	mov eax, 74
	mov r11, rbp[-0x8]
	mov r11[0x12c], eax

	mov eax, 75
	mov r11, rbp[-0x8]
	mov r11[0x130], eax

	mov eax, 76
	mov r11, rbp[-0x8]
	mov r11[0x134], eax

	mov eax, 77
	mov r11, rbp[-0x8]
	mov r11[0x138], eax

	mov eax, 78
	mov r11, rbp[-0x8]
	mov r11[0x13c], eax

	mov eax, 79
	mov r11, rbp[-0x8]
	mov r11[0x140], eax

	mov eax, 80
	mov r11, rbp[-0x8]
	mov r11[0x144], eax

	mov eax, 81
	mov r11, rbp[-0x8]
	mov r11[0x148], eax

	mov eax, 82
	mov r11, rbp[-0x8]
	mov r11[0x14c], eax

	mov eax, 83
	mov r11, rbp[-0x8]
	mov r11[0x150], eax

	mov eax, 84
	mov r11, rbp[-0x8]
	mov r11[0x154], eax

	mov eax, 85
	mov r11, rbp[-0x8]
	mov r11[0x158], eax

	mov eax, 86
	mov r11, rbp[-0x8]
	mov r11[0x15c], eax

	mov eax, 87
	mov r11, rbp[-0x8]
	mov r11[0x160], eax

	mov eax, 88
	mov r11, rbp[-0x8]
	mov r11[0x164], eax

	mov eax, 89
	mov r11, rbp[-0x8]
	mov r11[0x168], eax

	mov eax, 90
	mov r11, rbp[-0x8]
	mov r11[0x16c], eax

	mov eax, 91
	mov r11, rbp[-0x8]
	mov r11[0x170], eax

	mov eax, 92
	mov r11, rbp[-0x8]
	mov r11[0x174], eax

	mov eax, 93
	mov r11, rbp[-0x8]
	mov r11[0x178], eax

	mov eax, 94
	mov r11, rbp[-0x8]
	mov r11[0x17c], eax

	mov eax, 95
	mov r11, rbp[-0x8]
	mov r11[0x180], eax

	mov eax, 96
	mov r11, rbp[-0x8]
	mov r11[0x184], eax

	mov eax, 97
	mov r11, rbp[-0x8]
	mov r11[0x188], eax

	mov eax, 98
	mov r11, rbp[-0x8]
	mov r11[0x18c], eax

	mov eax, 99
	mov r11, rbp[-0x8]
	mov r11[0x190], eax

	mov eax, 100
	mov r11, rbp[-0x8]
	mov r11[0x194], eax

	mov eax, 101
	mov r11, rbp[-0x8]
	mov r11[0x198], eax

	mov eax, 102
	mov r11, rbp[-0x8]
	mov r11[0x19c], eax

	mov eax, 103
	mov r11, rbp[-0x8]
	mov r11[0x1a0], eax

	mov eax, 104
	mov r11, rbp[-0x8]
	mov r11[0x1a4], eax

	mov eax, 105
	mov r11, rbp[-0x8]
	mov r11[0x1a8], eax

	mov eax, 106
	mov r11, rbp[-0x8]
	mov r11[0x1ac], eax

	mov eax, 107
	mov r11, rbp[-0x8]
	mov r11[0x1b0], eax

	mov eax, 108
	mov r11, rbp[-0x8]
	mov r11[0x1b4], eax

	mov eax, 109
	mov r11, rbp[-0x8]
	mov r11[0x1b8], eax

	mov eax, 110
	mov r11, rbp[-0x8]
	mov r11[0x1bc], eax

	mov eax, 111
	mov r11, rbp[-0x8]
	mov r11[0x1c0], eax

	mov eax, 112
	mov r11, rbp[-0x8]
	mov r11[0x1c4], eax

	mov eax, 113
	mov r11, rbp[-0x8]
	mov r11[0x1c8], eax

	mov eax, 114
	mov r11, rbp[-0x8]
	mov r11[0x1cc], eax

	mov eax, 115
	mov r11, rbp[-0x8]
	mov r11[0x1d0], eax

	mov eax, 116
	mov r11, rbp[-0x8]
	mov r11[0x1d4], eax

	mov eax, 117
	mov r11, rbp[-0x8]
	mov r11[0x1d8], eax

	mov eax, 118
	mov r11, rbp[-0x8]
	mov r11[0x1dc], eax

	mov eax, 119
	mov r11, rbp[-0x8]
	mov r11[0x1e0], eax

	mov eax, 120
	mov r11, rbp[-0x8]
	mov r11[0x1e4], eax

	mov eax, 121
	mov r11, rbp[-0x8]
	mov r11[0x1e8], eax

	mov eax, 122
	mov r11, rbp[-0x8]
	mov r11[0x1ec], eax

	mov eax, 123
	mov r11, rbp[-0x8]
	mov r11[0x1f0], eax

	mov eax, 124
	mov r11, rbp[-0x8]
	mov r11[0x1f4], eax

	mov eax, 125
	mov r11, rbp[-0x8]
	mov r11[0x1f8], eax

	mov eax, 126
	mov r11, rbp[-0x8]
	mov r11[0x1fc], eax

	mov eax, 127
	mov r11, rbp[-0x8]
	mov r11[0x200], eax

	mov eax, 128
	mov r11, rbp[-0x8]
	mov r11[0x204], eax

	mov eax, 129
	mov r11, rbp[-0x8]
	mov r11[0x208], eax

	mov eax, 130
	mov r11, rbp[-0x8]
	mov r11[0x20c], eax

	mov eax, 131
	mov r11, rbp[-0x8]
	mov r11[0x210], eax

	mov eax, 132
	mov r11, rbp[-0x8]
	mov r11[0x214], eax

	mov eax, 133
	mov r11, rbp[-0x8]
	mov r11[0x218], eax

	mov eax, 134
	mov r11, rbp[-0x8]
	mov r11[0x21c], eax

	mov eax, 135
	mov r11, rbp[-0x8]
	mov r11[0x220], eax

	mov eax, 136
	mov r11, rbp[-0x8]
	mov r11[0x224], eax

	mov eax, 137
	mov r11, rbp[-0x8]
	mov r11[0x228], eax

	mov eax, 138
	mov r11, rbp[-0x8]
	mov r11[0x22c], eax

	mov eax, 139
	mov r11, rbp[-0x8]
	mov r11[0x230], eax

	mov eax, 140
	mov r11, rbp[-0x8]
	mov r11[0x234], eax

	mov eax, 141
	mov r11, rbp[-0x8]
	mov r11[0x238], eax

	mov eax, 142
	mov r11, rbp[-0x8]
	mov r11[0x23c], eax

	mov eax, 143
	mov r11, rbp[-0x8]
	mov r11[0x240], eax

	mov eax, 144
	mov r11, rbp[-0x8]
	mov r11[0x244], eax

	mov eax, 145
	mov r11, rbp[-0x8]
	mov r11[0x248], eax

	mov eax, 146
	mov r11, rbp[-0x8]
	mov r11[0x24c], eax

	mov eax, 147
	mov r11, rbp[-0x8]
	mov r11[0x250], eax

	mov eax, 148
	mov r11, rbp[-0x8]
	mov r11[0x254], eax

	mov eax, 149
	mov r11, rbp[-0x8]
	mov r11[0x258], eax

	mov eax, 150
	mov r11, rbp[-0x8]
	mov r11[0x25c], eax

	mov eax, 151
	mov r11, rbp[-0x8]
	mov r11[0x260], eax

	mov eax, 152
	mov r11, rbp[-0x8]
	mov r11[0x264], eax

	mov eax, 153
	mov r11, rbp[-0x8]
	mov r11[0x268], eax

	mov eax, 154
	mov r11, rbp[-0x8]
	mov r11[0x26c], eax

	mov eax, 155
	mov r11, rbp[-0x8]
	mov r11[0x270], eax

	mov eax, 156
	mov r11, rbp[-0x8]
	mov r11[0x274], eax

	mov eax, 157
	mov r11, rbp[-0x8]
	mov r11[0x278], eax

	mov eax, 158
	mov r11, rbp[-0x8]
	mov r11[0x27c], eax

	mov eax, 159
	mov r11, rbp[-0x8]
	mov r11[0x280], eax

	mov eax, 160
	mov r11, rbp[-0x8]
	mov r11[0x284], eax

	mov eax, 161
	mov r11, rbp[-0x8]
	mov r11[0x288], eax

	mov eax, 162
	mov r11, rbp[-0x8]
	mov r11[0x28c], eax

	mov eax, 163
	mov r11, rbp[-0x8]
	mov r11[0x290], eax

	mov eax, 164
	mov r11, rbp[-0x8]
	mov r11[0x294], eax

	mov eax, 165
	mov r11, rbp[-0x8]
	mov r11[0x298], eax

	mov eax, 166
	mov r11, rbp[-0x8]
	mov r11[0x29c], eax

	mov eax, 167
	mov r11, rbp[-0x8]
	mov r11[0x2a0], eax

	mov eax, 168
	mov r11, rbp[-0x8]
	mov r11[0x2a4], eax

	mov eax, 169
	mov r11, rbp[-0x8]
	mov r11[0x2a8], eax

	mov eax, 170
	mov r11, rbp[-0x8]
	mov r11[0x2ac], eax

	mov eax, 171
	mov r11, rbp[-0x8]
	mov r11[0x2b0], eax

	mov eax, 172
	mov r11, rbp[-0x8]
	mov r11[0x2b4], eax

	mov eax, 173
	mov r11, rbp[-0x8]
	mov r11[0x2b8], eax

	mov eax, 174
	mov r11, rbp[-0x8]
	mov r11[0x2bc], eax

	mov eax, 175
	mov r11, rbp[-0x8]
	mov r11[0x2c0], eax

	mov eax, 176
	mov r11, rbp[-0x8]
	mov r11[0x2c4], eax

	mov eax, 177
	mov r11, rbp[-0x8]
	mov r11[0x2c8], eax

	mov eax, 178
	mov r11, rbp[-0x8]
	mov r11[0x2cc], eax

	mov eax, 179
	mov r11, rbp[-0x8]
	mov r11[0x2d0], eax

	mov eax, 180
	mov r11, rbp[-0x8]
	mov r11[0x2d4], eax

	mov eax, 181
	mov r11, rbp[-0x8]
	mov r11[0x2d8], eax

	mov eax, 182
	mov r11, rbp[-0x8]
	mov r11[0x2dc], eax

	mov eax, 183
	mov r11, rbp[-0x8]
	mov r11[0x2e0], eax

	mov eax, 184
	mov r11, rbp[-0x8]
	mov r11[0x2e4], eax

	mov eax, 185
	mov r11, rbp[-0x8]
	mov r11[0x2e8], eax

	mov eax, 186
	mov r11, rbp[-0x8]
	mov r11[0x2ec], eax

	mov eax, 187
	mov r11, rbp[-0x8]
	mov r11[0x2f0], eax

	mov eax, 188
	mov r11, rbp[-0x8]
	mov r11[0x2f4], eax

	mov eax, 189
	mov r11, rbp[-0x8]
	mov r11[0x2f8], eax

	mov eax, 190
	mov r11, rbp[-0x8]
	mov r11[0x2fc], eax

	mov eax, 191
	mov r11, rbp[-0x8]
	mov r11[0x300], eax

	mov eax, 192
	mov r11, rbp[-0x8]
	mov r11[0x304], eax

	mov eax, 193
	mov r11, rbp[-0x8]
	mov r11[0x308], eax

	mov eax, 194
	mov r11, rbp[-0x8]
	mov r11[0x30c], eax

	mov eax, 195
	mov r11, rbp[-0x8]
	mov r11[0x310], eax

	mov eax, 196
	mov r11, rbp[-0x8]
	mov r11[0x314], eax

	mov eax, 197
	mov r11, rbp[-0x8]
	mov r11[0x318], eax

	mov eax, 198
	mov r11, rbp[-0x8]
	mov r11[0x31c], eax

	mov eax, 199
	mov r11, rbp[-0x8]
	mov r11[0x320], eax

	mov eax, 200
	mov r11, rbp[-0x8]
	mov r11[0x324], eax

	mov eax, 201
	mov r11, rbp[-0x8]
	mov r11[0x328], eax

	mov eax, 202
	mov r11, rbp[-0x8]
	mov r11[0x32c], eax

	mov eax, 203
	mov r11, rbp[-0x8]
	mov r11[0x330], eax

	mov eax, 204
	mov r11, rbp[-0x8]
	mov r11[0x334], eax

	mov eax, 205
	mov r11, rbp[-0x8]
	mov r11[0x338], eax

	mov eax, 206
	mov r11, rbp[-0x8]
	mov r11[0x33c], eax

	mov eax, 207
	mov r11, rbp[-0x8]
	mov r11[0x340], eax

	mov eax, 208
	mov r11, rbp[-0x8]
	mov r11[0x344], eax

	mov eax, 209
	mov r11, rbp[-0x8]
	mov r11[0x348], eax

	mov eax, 210
	mov r11, rbp[-0x8]
	mov r11[0x34c], eax

	mov eax, 211
	mov r11, rbp[-0x8]
	mov r11[0x350], eax

	mov eax, 212
	mov r11, rbp[-0x8]
	mov r11[0x354], eax

	mov eax, 213
	mov r11, rbp[-0x8]
	mov r11[0x358], eax

	mov eax, 214
	mov r11, rbp[-0x8]
	mov r11[0x35c], eax

	mov eax, 215
	mov r11, rbp[-0x8]
	mov r11[0x360], eax

	mov eax, 216
	mov r11, rbp[-0x8]
	mov r11[0x364], eax

	mov eax, 217
	mov r11, rbp[-0x8]
	mov r11[0x368], eax

	mov eax, 218
	mov r11, rbp[-0x8]
	mov r11[0x36c], eax

	mov eax, 219
	mov r11, rbp[-0x8]
	mov r11[0x370], eax

	mov eax, 220
	mov r11, rbp[-0x8]
	mov r11[0x374], eax

	mov eax, 221
	mov r11, rbp[-0x8]
	mov r11[0x378], eax

	mov eax, 222
	mov r11, rbp[-0x8]
	mov r11[0x37c], eax

	mov eax, 223
	mov r11, rbp[-0x8]
	mov r11[0x380], eax

	mov eax, 224
	mov r11, rbp[-0x8]
	mov r11[0x384], eax

	mov eax, 225
	mov r11, rbp[-0x8]
	mov r11[0x388], eax

	mov eax, 226
	mov r11, rbp[-0x8]
	mov r11[0x38c], eax

	mov eax, 227
	mov r11, rbp[-0x8]
	mov r11[0x390], eax

	mov eax, 228
	mov r11, rbp[-0x8]
	mov r11[0x394], eax

	mov eax, 229
	mov r11, rbp[-0x8]
	mov r11[0x398], eax

	mov eax, 230
	mov r11, rbp[-0x8]
	mov r11[0x39c], eax

	mov eax, 231
	mov r11, rbp[-0x8]
	mov r11[0x3a0], eax

	mov eax, 232
	mov r11, rbp[-0x8]
	mov r11[0x3a4], eax

	mov eax, 233
	mov r11, rbp[-0x8]
	mov r11[0x3a8], eax

	mov eax, 234
	mov r11, rbp[-0x8]
	mov r11[0x3ac], eax

	mov eax, 235
	mov r11, rbp[-0x8]
	mov r11[0x3b0], eax

	mov eax, 236
	mov r11, rbp[-0x8]
	mov r11[0x3b4], eax

	mov eax, 237
	mov r11, rbp[-0x8]
	mov r11[0x3b8], eax

	mov eax, 238
	mov r11, rbp[-0x8]
	mov r11[0x3bc], eax

	mov eax, 239
	mov r11, rbp[-0x8]
	mov r11[0x3c0], eax

	mov eax, 240
	mov r11, rbp[-0x8]
	mov r11[0x3c4], eax

	mov eax, 241
	mov r11, rbp[-0x8]
	mov r11[0x3c8], eax

	mov eax, 242
	mov r11, rbp[-0x8]
	mov r11[0x3cc], eax

	mov eax, 243
	mov r11, rbp[-0x8]
	mov r11[0x3d0], eax

	mov eax, 244
	mov r11, rbp[-0x8]
	mov r11[0x3d4], eax

	mov eax, 245
	mov r11, rbp[-0x8]
	mov r11[0x3d8], eax

	mov eax, 246
	mov r11, rbp[-0x8]
	mov r11[0x3dc], eax

	mov eax, 247
	mov r11, rbp[-0x8]
	mov r11[0x3e0], eax

	mov eax, 248
	mov r11, rbp[-0x8]
	mov r11[0x3e4], eax

	mov eax, 249
	mov r11, rbp[-0x8]
	mov r11[0x3e8], eax

	mov eax, 250
	mov r11, rbp[-0x8]
	mov r11[0x3ec], eax

	mov eax, 251
	mov r11, rbp[-0x8]
	mov r11[0x3f0], eax

	mov eax, 252
	mov r11, rbp[-0x8]
	mov r11[0x3f4], eax

	mov eax, 253
	mov r11, rbp[-0x8]
	mov r11[0x3f8], eax

	mov eax, 254
	mov r11, rbp[-0x8]
	mov r11[0x3fc], eax

	mov eax, 255
	mov r11, rbp[-0x8]
	mov r11[0x400], eax

	mov eax, 256
	mov r11, rbp[-0x8]
	mov r11[0x404], eax

	mov eax, 257
	mov r11, rbp[-0x8]
	mov r11[0x408], eax

	mov eax, 258
	mov r11, rbp[-0x8]
	mov r11[0x40c], eax

	mov eax, 259
	mov r11, rbp[-0x8]
	mov r11[0x410], eax

	mov eax, 260
	mov r11, rbp[-0x8]
	mov r11[0x414], eax

	mov eax, 261
	mov r11, rbp[-0x8]
	mov r11[0x418], eax

	mov eax, 262
	mov r11, rbp[-0x8]
	mov r11[0x41c], eax

	mov eax, 263
	mov r11, rbp[-0x8]
	mov r11[0x420], eax

	mov eax, 264
	mov r11, rbp[-0x8]
	mov r11[0x424], eax

	mov eax, 265
	mov r11, rbp[-0x8]
	mov r11[0x428], eax

	mov eax, 266
	mov r11, rbp[-0x8]
	mov r11[0x42c], eax

	mov eax, 267
	mov r11, rbp[-0x8]
	mov r11[0x430], eax

	mov eax, 268
	mov r11, rbp[-0x8]
	mov r11[0x434], eax

	mov eax, 269
	mov r11, rbp[-0x8]
	mov r11[0x438], eax

	mov eax, 270
	mov r11, rbp[-0x8]
	mov r11[0x43c], eax

	mov eax, 271
	mov r11, rbp[-0x8]
	mov r11[0x440], eax

	mov eax, 272
	mov r11, rbp[-0x8]
	mov r11[0x444], eax

	mov eax, 273
	mov r11, rbp[-0x8]
	mov r11[0x448], eax

	mov eax, 274
	mov r11, rbp[-0x8]
	mov r11[0x44c], eax

	mov eax, 275
	mov r11, rbp[-0x8]
	mov r11[0x450], eax

	mov eax, 276
	mov r11, rbp[-0x8]
	mov r11[0x454], eax

	mov eax, 277
	mov r11, rbp[-0x8]
	mov r11[0x458], eax

	mov eax, 278
	mov r11, rbp[-0x8]
	mov r11[0x45c], eax

	mov eax, 279
	mov r11, rbp[-0x8]
	mov r11[0x460], eax

	mov eax, 280
	mov r11, rbp[-0x8]
	mov r11[0x464], eax

	mov eax, 281
	mov r11, rbp[-0x8]
	mov r11[0x468], eax

	mov eax, 282
	mov r11, rbp[-0x8]
	mov r11[0x46c], eax

	mov eax, 283
	mov r11, rbp[-0x8]
	mov r11[0x470], eax

	mov eax, 284
	mov r11, rbp[-0x8]
	mov r11[0x474], eax

	mov eax, 285
	mov r11, rbp[-0x8]
	mov r11[0x478], eax

	mov eax, 286
	mov r11, rbp[-0x8]
	mov r11[0x47c], eax

	mov eax, 287
	mov r11, rbp[-0x8]
	mov r11[0x480], eax

	mov eax, 288
	mov r11, rbp[-0x8]
	mov r11[0x484], eax

	mov eax, 289
	mov r11, rbp[-0x8]
	mov r11[0x488], eax

	mov eax, 290
	mov r11, rbp[-0x8]
	mov r11[0x48c], eax

	mov eax, 291
	mov r11, rbp[-0x8]
	mov r11[0x490], eax

	mov eax, 292
	mov r11, rbp[-0x8]
	mov r11[0x494], eax

	mov eax, 293
	mov r11, rbp[-0x8]
	mov r11[0x498], eax

	mov eax, 294
	mov r11, rbp[-0x8]
	mov r11[0x49c], eax

	mov eax, 295
	mov r11, rbp[-0x8]
	mov r11[0x4a0], eax

	mov eax, 296
	mov r11, rbp[-0x8]
	mov r11[0x4a4], eax

	mov eax, 297
	mov r11, rbp[-0x8]
	mov r11[0x4a8], eax

	mov eax, 298
	mov r11, rbp[-0x8]
	mov r11[0x4ac], eax

	mov eax, 299
	mov r11, rbp[-0x8]
	mov r11[0x4b0], eax

	mov eax, 300
	mov r11, rbp[-0x8]
	mov r11[0x4b4], eax

	mov eax, 301
	mov r11, rbp[-0x8]
	mov r11[0x4b8], eax

	mov eax, 302
	mov r11, rbp[-0x8]
	mov r11[0x4bc], eax

	mov eax, 303
	mov r11, rbp[-0x8]
	mov r11[0x4c0], eax

	mov eax, 304
	mov r11, rbp[-0x8]
	mov r11[0x4c4], eax

	mov eax, 305
	mov r11, rbp[-0x8]
	mov r11[0x4c8], eax

	mov eax, 306
	mov r11, rbp[-0x8]
	mov r11[0x4cc], eax

	mov eax, 307
	mov r11, rbp[-0x8]
	mov r11[0x4d0], eax

	mov eax, 308
	mov r11, rbp[-0x8]
	mov r11[0x4d4], eax

	mov eax, 309
	mov r11, rbp[-0x8]
	mov r11[0x4d8], eax

	mov eax, 310
	mov r11, rbp[-0x8]
	mov r11[0x4dc], eax

	mov eax, 311
	mov r11, rbp[-0x8]
	mov r11[0x4e0], eax

	mov eax, 312
	mov r11, rbp[-0x8]
	mov r11[0x4e4], eax

	mov eax, 313
	mov r11, rbp[-0x8]
	mov r11[0x4e8], eax

	mov eax, 314
	mov r11, rbp[-0x8]
	mov r11[0x4ec], eax

	mov eax, 315
	mov r11, rbp[-0x8]
	mov r11[0x4f0], eax

	mov eax, 316
	mov r11, rbp[-0x8]
	mov r11[0x4f4], eax

	mov eax, 317
	mov r11, rbp[-0x8]
	mov r11[0x4f8], eax

	mov eax, 318
	mov r11, rbp[-0x8]
	mov r11[0x4fc], eax

	mov eax, 319
	mov r11, rbp[-0x8]
	mov r11[0x500], eax

	mov eax, 320
	mov r11, rbp[-0x8]
	mov r11[0x504], eax

	mov eax, 321
	mov r11, rbp[-0x8]
	mov r11[0x508], eax

	mov eax, 322
	mov r11, rbp[-0x8]
	mov r11[0x50c], eax

	mov eax, 323
	mov r11, rbp[-0x8]
	mov r11[0x510], eax

	mov eax, 324
	mov r11, rbp[-0x8]
	mov r11[0x514], eax

	mov eax, 325
	mov r11, rbp[-0x8]
	mov r11[0x518], eax

	mov eax, 326
	mov r11, rbp[-0x8]
	mov r11[0x51c], eax

	mov eax, 327
	mov r11, rbp[-0x8]
	mov r11[0x520], eax

	mov eax, 328
	mov r11, rbp[-0x8]
	mov r11[0x524], eax

	mov eax, 329
	mov r11, rbp[-0x8]
	mov r11[0x528], eax

	mov eax, 330
	mov r11, rbp[-0x8]
	mov r11[0x52c], eax

	mov eax, 331
	mov r11, rbp[-0x8]
	mov r11[0x530], eax

	mov eax, 332
	mov r11, rbp[-0x8]
	mov r11[0x534], eax

	mov eax, 333
	mov r11, rbp[-0x8]
	mov r11[0x538], eax

	mov eax, 334
	mov r11, rbp[-0x8]
	mov r11[0x53c], eax

	mov eax, 335
	mov r11, rbp[-0x8]
	mov r11[0x540], eax

	mov eax, 336
	mov r11, rbp[-0x8]
	mov r11[0x544], eax

	mov eax, 337
	mov r11, rbp[-0x8]
	mov r11[0x548], eax

	mov eax, 338
	mov r11, rbp[-0x8]
	mov r11[0x54c], eax

	mov eax, 339
	mov r11, rbp[-0x8]
	mov r11[0x550], eax

	mov eax, 340
	mov r11, rbp[-0x8]
	mov r11[0x554], eax

	mov eax, 341
	mov r11, rbp[-0x8]
	mov r11[0x558], eax

	mov eax, 342
	mov r11, rbp[-0x8]
	mov r11[0x55c], eax

	mov eax, 343
	mov r11, rbp[-0x8]
	mov r11[0x560], eax

	mov eax, 344
	mov r11, rbp[-0x8]
	mov r11[0x564], eax

	mov eax, 345
	mov r11, rbp[-0x8]
	mov r11[0x568], eax

	mov eax, 346
	mov r11, rbp[-0x8]
	mov r11[0x56c], eax

	mov eax, 347
	mov r11, rbp[-0x8]
	mov r11[0x570], eax

	mov eax, 348
	mov r11, rbp[-0x8]
	mov r11[0x574], eax

	mov eax, 349
	mov r11, rbp[-0x8]
	mov r11[0x578], eax

	mov eax, 350
	mov r11, rbp[-0x8]
	mov r11[0x57c], eax

	mov eax, 351
	mov r11, rbp[-0x8]
	mov r11[0x580], eax

	mov eax, 352
	mov r11, rbp[-0x8]
	mov r11[0x584], eax

	mov eax, 353
	mov r11, rbp[-0x8]
	mov r11[0x588], eax

	mov eax, 354
	mov r11, rbp[-0x8]
	mov r11[0x58c], eax

	mov eax, 355
	mov r11, rbp[-0x8]
	mov r11[0x590], eax

	mov eax, 356
	mov r11, rbp[-0x8]
	mov r11[0x594], eax

	mov eax, 357
	mov r11, rbp[-0x8]
	mov r11[0x598], eax

	mov eax, 358
	mov r11, rbp[-0x8]
	mov r11[0x59c], eax

	mov eax, 359
	mov r11, rbp[-0x8]
	mov r11[0x5a0], eax

	mov eax, 360
	mov r11, rbp[-0x8]
	mov r11[0x5a4], eax

	mov eax, 361
	mov r11, rbp[-0x8]
	mov r11[0x5a8], eax

	mov eax, 362
	mov r11, rbp[-0x8]
	mov r11[0x5ac], eax

	mov eax, 363
	mov r11, rbp[-0x8]
	mov r11[0x5b0], eax

	mov eax, 364
	mov r11, rbp[-0x8]
	mov r11[0x5b4], eax

	mov eax, 365
	mov r11, rbp[-0x8]
	mov r11[0x5b8], eax

	mov eax, 366
	mov r11, rbp[-0x8]
	mov r11[0x5bc], eax

	mov eax, 367
	mov r11, rbp[-0x8]
	mov r11[0x5c0], eax

	mov eax, 368
	mov r11, rbp[-0x8]
	mov r11[0x5c4], eax

	mov eax, 369
	mov r11, rbp[-0x8]
	mov r11[0x5c8], eax

	mov eax, 370
	mov r11, rbp[-0x8]
	mov r11[0x5cc], eax

	mov eax, 371
	mov r11, rbp[-0x8]
	mov r11[0x5d0], eax

	mov eax, 372
	mov r11, rbp[-0x8]
	mov r11[0x5d4], eax

	mov eax, 373
	mov r11, rbp[-0x8]
	mov r11[0x5d8], eax

	mov eax, 374
	mov r11, rbp[-0x8]
	mov r11[0x5dc], eax

	mov eax, 375
	mov r11, rbp[-0x8]
	mov r11[0x5e0], eax

	mov eax, 376
	mov r11, rbp[-0x8]
	mov r11[0x5e4], eax

	mov eax, 377
	mov r11, rbp[-0x8]
	mov r11[0x5e8], eax

	mov eax, 378
	mov r11, rbp[-0x8]
	mov r11[0x5ec], eax

	mov eax, 379
	mov r11, rbp[-0x8]
	mov r11[0x5f0], eax

	mov eax, 380
	mov r11, rbp[-0x8]
	mov r11[0x5f4], eax

	mov eax, 381
	mov r11, rbp[-0x8]
	mov r11[0x5f8], eax

	mov eax, 382
	mov r11, rbp[-0x8]
	mov r11[0x5fc], eax

	mov eax, 383
	mov r11, rbp[-0x8]
	mov r11[0x600], eax

	mov eax, 384
	mov r11, rbp[-0x8]
	mov r11[0x604], eax

	mov eax, 385
	mov r11, rbp[-0x8]
	mov r11[0x608], eax

	mov eax, 386
	mov r11, rbp[-0x8]
	mov r11[0x60c], eax

	mov eax, 387
	mov r11, rbp[-0x8]
	mov r11[0x610], eax

	mov eax, 388
	mov r11, rbp[-0x8]
	mov r11[0x614], eax

	mov eax, 389
	mov r11, rbp[-0x8]
	mov r11[0x618], eax

	mov eax, 390
	mov r11, rbp[-0x8]
	mov r11[0x61c], eax

	mov eax, 391
	mov r11, rbp[-0x8]
	mov r11[0x620], eax

	mov eax, 392
	mov r11, rbp[-0x8]
	mov r11[0x624], eax

	mov eax, 393
	mov r11, rbp[-0x8]
	mov r11[0x628], eax

	mov eax, 394
	mov r11, rbp[-0x8]
	mov r11[0x62c], eax

	mov eax, 395
	mov r11, rbp[-0x8]
	mov r11[0x630], eax

	mov eax, 396
	mov r11, rbp[-0x8]
	mov r11[0x634], eax

	mov eax, 397
	mov r11, rbp[-0x8]
	mov r11[0x638], eax

	mov eax, 398
	mov r11, rbp[-0x8]
	mov r11[0x63c], eax

	mov eax, 399
	mov r11, rbp[-0x8]
	mov r11[0x640], eax

	mov eax, 400
	mov r11, rbp[-0x8]
	mov r11[0x644], eax

	mov eax, 401
	mov r11, rbp[-0x8]
	mov r11[0x648], eax

	mov eax, 402
	mov r11, rbp[-0x8]
	mov r11[0x64c], eax

	mov eax, 403
	mov r11, rbp[-0x8]
	mov r11[0x650], eax

	mov eax, 404
	mov r11, rbp[-0x8]
	mov r11[0x654], eax

	mov eax, 405
	mov r11, rbp[-0x8]
	mov r11[0x658], eax

	mov eax, 406
	mov r11, rbp[-0x8]
	mov r11[0x65c], eax

	mov eax, 407
	mov r11, rbp[-0x8]
	mov r11[0x660], eax

	mov eax, 408
	mov r11, rbp[-0x8]
	mov r11[0x664], eax

	mov eax, 409
	mov r11, rbp[-0x8]
	mov r11[0x668], eax

	mov eax, 410
	mov r11, rbp[-0x8]
	mov r11[0x66c], eax

	mov eax, 411
	mov r11, rbp[-0x8]
	mov r11[0x670], eax

	mov eax, 412
	mov r11, rbp[-0x8]
	mov r11[0x674], eax

	mov eax, 413
	mov r11, rbp[-0x8]
	mov r11[0x678], eax

	mov eax, 414
	mov r11, rbp[-0x8]
	mov r11[0x67c], eax

	mov eax, 415
	mov r11, rbp[-0x8]
	mov r11[0x680], eax

	mov eax, 416
	mov r11, rbp[-0x8]
	mov r11[0x684], eax

	mov eax, 417
	mov r11, rbp[-0x8]
	mov r11[0x688], eax

	mov eax, 418
	mov r11, rbp[-0x8]
	mov r11[0x68c], eax

	mov eax, 419
	mov r11, rbp[-0x8]
	mov r11[0x690], eax

	mov eax, 420
	mov r11, rbp[-0x8]
	mov r11[0x694], eax

	mov eax, 421
	mov r11, rbp[-0x8]
	mov r11[0x698], eax

	mov eax, 422
	mov r11, rbp[-0x8]
	mov r11[0x69c], eax

	mov eax, 423
	mov r11, rbp[-0x8]
	mov r11[0x6a0], eax

	mov eax, 424
	mov r11, rbp[-0x8]
	mov r11[0x6a4], eax

	mov eax, 425
	mov r11, rbp[-0x8]
	mov r11[0x6a8], eax

	mov eax, 426
	mov r11, rbp[-0x8]
	mov r11[0x6ac], eax

	mov eax, 427
	mov r11, rbp[-0x8]
	mov r11[0x6b0], eax

	mov eax, 428
	mov r11, rbp[-0x8]
	mov r11[0x6b4], eax

	mov eax, 429
	mov r11, rbp[-0x8]
	mov r11[0x6b8], eax

	mov eax, 430
	mov r11, rbp[-0x8]
	mov r11[0x6bc], eax

	mov eax, 431
	mov r11, rbp[-0x8]
	mov r11[0x6c0], eax

	mov eax, 432
	mov r11, rbp[-0x8]
	mov r11[0x6c4], eax

	mov eax, 433
	mov r11, rbp[-0x8]
	mov r11[0x6c8], eax

	mov eax, 434
	mov r11, rbp[-0x8]
	mov r11[0x6cc], eax

	mov eax, 435
	mov r11, rbp[-0x8]
	mov r11[0x6d0], eax

	mov eax, 436
	mov r11, rbp[-0x8]
	mov r11[0x6d4], eax

	mov eax, 437
	mov r11, rbp[-0x8]
	mov r11[0x6d8], eax

	mov eax, 438
	mov r11, rbp[-0x8]
	mov r11[0x6dc], eax

	mov eax, 439
	mov r11, rbp[-0x8]
	mov r11[0x6e0], eax

	mov eax, 440
	mov r11, rbp[-0x8]
	mov r11[0x6e4], eax

	mov eax, 441
	mov r11, rbp[-0x8]
	mov r11[0x6e8], eax

	mov eax, 442
	mov r11, rbp[-0x8]
	mov r11[0x6ec], eax

	mov eax, 443
	mov r11, rbp[-0x8]
	mov r11[0x6f0], eax

	mov eax, 444
	mov r11, rbp[-0x8]
	mov r11[0x6f4], eax

	mov eax, 445
	mov r11, rbp[-0x8]
	mov r11[0x6f8], eax

	mov eax, 446
	mov r11, rbp[-0x8]
	mov r11[0x6fc], eax

	mov eax, 447
	mov r11, rbp[-0x8]
	mov r11[0x700], eax

	mov eax, 448
	mov r11, rbp[-0x8]
	mov r11[0x704], eax

	mov eax, 449
	mov r11, rbp[-0x8]
	mov r11[0x708], eax

	mov eax, 450
	mov r11, rbp[-0x8]
	mov r11[0x70c], eax

	mov eax, 451
	mov r11, rbp[-0x8]
	mov r11[0x710], eax

	mov eax, 452
	mov r11, rbp[-0x8]
	mov r11[0x714], eax

	mov eax, 453
	mov r11, rbp[-0x8]
	mov r11[0x718], eax

	mov eax, 454
	mov r11, rbp[-0x8]
	mov r11[0x71c], eax

	mov eax, 455
	mov r11, rbp[-0x8]
	mov r11[0x720], eax

	mov eax, 456
	mov r11, rbp[-0x8]
	mov r11[0x724], eax

	mov eax, 457
	mov r11, rbp[-0x8]
	mov r11[0x728], eax

	mov eax, 458
	mov r11, rbp[-0x8]
	mov r11[0x72c], eax

	mov eax, 459
	mov r11, rbp[-0x8]
	mov r11[0x730], eax

	mov eax, 460
	mov r11, rbp[-0x8]
	mov r11[0x734], eax

	mov eax, 461
	mov r11, rbp[-0x8]
	mov r11[0x738], eax

	mov eax, 462
	mov r11, rbp[-0x8]
	mov r11[0x73c], eax

	mov eax, 463
	mov r11, rbp[-0x8]
	mov r11[0x740], eax

	mov eax, 464
	mov r11, rbp[-0x8]
	mov r11[0x744], eax

	mov eax, 465
	mov r11, rbp[-0x8]
	mov r11[0x748], eax

	mov eax, 466
	mov r11, rbp[-0x8]
	mov r11[0x74c], eax

	mov eax, 467
	mov r11, rbp[-0x8]
	mov r11[0x750], eax

	mov eax, 468
	mov r11, rbp[-0x8]
	mov r11[0x754], eax

	mov eax, 469
	mov r11, rbp[-0x8]
	mov r11[0x758], eax

	mov eax, 470
	mov r11, rbp[-0x8]
	mov r11[0x75c], eax

	mov eax, 471
	mov r11, rbp[-0x8]
	mov r11[0x760], eax

	mov eax, 472
	mov r11, rbp[-0x8]
	mov r11[0x764], eax

	mov eax, 473
	mov r11, rbp[-0x8]
	mov r11[0x768], eax

	mov eax, 474
	mov r11, rbp[-0x8]
	mov r11[0x76c], eax

	mov eax, 475
	mov r11, rbp[-0x8]
	mov r11[0x770], eax

	mov eax, 476
	mov r11, rbp[-0x8]
	mov r11[0x774], eax

	mov eax, 477
	mov r11, rbp[-0x8]
	mov r11[0x778], eax

	mov eax, 478
	mov r11, rbp[-0x8]
	mov r11[0x77c], eax

	mov eax, 479
	mov r11, rbp[-0x8]
	mov r11[0x780], eax

	mov eax, 480
	mov r11, rbp[-0x8]
	mov r11[0x784], eax

	mov eax, 481
	mov r11, rbp[-0x8]
	mov r11[0x788], eax

	mov eax, 482
	mov r11, rbp[-0x8]
	mov r11[0x78c], eax

	mov eax, 483
	mov r11, rbp[-0x8]
	mov r11[0x790], eax

	mov eax, 484
	mov r11, rbp[-0x8]
	mov r11[0x794], eax

	mov eax, 485
	mov r11, rbp[-0x8]
	mov r11[0x798], eax

	mov eax, 486
	mov r11, rbp[-0x8]
	mov r11[0x79c], eax

	mov eax, 487
	mov r11, rbp[-0x8]
	mov r11[0x7a0], eax

	mov eax, 488
	mov r11, rbp[-0x8]
	mov r11[0x7a4], eax

	mov eax, 489
	mov r11, rbp[-0x8]
	mov r11[0x7a8], eax

	mov eax, 490
	mov r11, rbp[-0x8]
	mov r11[0x7ac], eax

	mov eax, 491
	mov r11, rbp[-0x8]
	mov r11[0x7b0], eax

	mov eax, 492
	mov r11, rbp[-0x8]
	mov r11[0x7b4], eax

	mov eax, 493
	mov r11, rbp[-0x8]
	mov r11[0x7b8], eax

	mov eax, 494
	mov r11, rbp[-0x8]
	mov r11[0x7bc], eax

	mov eax, 495
	mov r11, rbp[-0x8]
	mov r11[0x7c0], eax

	mov eax, 496
	mov r11, rbp[-0x8]
	mov r11[0x7c4], eax

	mov eax, 497
	mov r11, rbp[-0x8]
	mov r11[0x7c8], eax

	mov eax, 498
	mov r11, rbp[-0x8]
	mov r11[0x7cc], eax

	mov eax, 499
	mov r11, rbp[-0x8]
	mov r11[0x7d0], eax

	mov eax, 500
	mov r11, rbp[-0x8]
	mov r11[0x7d4], eax

	mov eax, 501
	mov r11, rbp[-0x8]
	mov r11[0x7d8], eax

	mov eax, 502
	mov r11, rbp[-0x8]
	mov r11[0x7dc], eax

	mov eax, 503
	mov r11, rbp[-0x8]
	mov r11[0x7e0], eax

	mov eax, 504
	mov r11, rbp[-0x8]
	mov r11[0x7e4], eax

	mov eax, 505
	mov r11, rbp[-0x8]
	mov r11[0x7e8], eax

	mov eax, 506
	mov r11, rbp[-0x8]
	mov r11[0x7ec], eax

	mov eax, 507
	mov r11, rbp[-0x8]
	mov r11[0x7f0], eax

	mov eax, 508
	mov r11, rbp[-0x8]
	mov r11[0x7f4], eax

	mov eax, 509
	mov r11, rbp[-0x8]
	mov r11[0x7f8], eax

	mov eax, 510
	mov r11, rbp[-0x8]
	mov r11[0x7fc], eax

	mov eax, 511
	mov r11, rbp[-0x8]
	mov r11[0x800], eax

	mov eax, 512
	mov r11, rbp[-0x8]
	mov r11[0x804], eax

	mov eax, 513
	mov r11, rbp[-0x8]
	mov r11[0x808], eax

	mov eax, 514
	mov r11, rbp[-0x8]
	mov r11[0x80c], eax

	mov eax, 515
	mov r11, rbp[-0x8]
	mov r11[0x810], eax

	mov eax, 516
	mov r11, rbp[-0x8]
	mov r11[0x814], eax

	mov eax, 517
	mov r11, rbp[-0x8]
	mov r11[0x818], eax

	mov eax, 518
	mov r11, rbp[-0x8]
	mov r11[0x81c], eax

	mov eax, 519
	mov r11, rbp[-0x8]
	mov r11[0x820], eax

	mov eax, 520
	mov r11, rbp[-0x8]
	mov r11[0x824], eax

	mov eax, 521
	mov r11, rbp[-0x8]
	mov r11[0x828], eax

	mov eax, 522
	mov r11, rbp[-0x8]
	mov r11[0x82c], eax

	mov eax, 523
	mov r11, rbp[-0x8]
	mov r11[0x830], eax

	mov eax, 524
	mov r11, rbp[-0x8]
	mov r11[0x834], eax

	mov eax, 525
	mov r11, rbp[-0x8]
	mov r11[0x838], eax

	mov eax, 526
	mov r11, rbp[-0x8]
	mov r11[0x83c], eax

	mov eax, 527
	mov r11, rbp[-0x8]
	mov r11[0x840], eax

	mov eax, 528
	mov r11, rbp[-0x8]
	mov r11[0x844], eax

	mov eax, 529
	mov r11, rbp[-0x8]
	mov r11[0x848], eax

	mov eax, 530
	mov r11, rbp[-0x8]
	mov r11[0x84c], eax

	mov eax, 531
	mov r11, rbp[-0x8]
	mov r11[0x850], eax

	mov eax, 532
	mov r11, rbp[-0x8]
	mov r11[0x854], eax

	mov eax, 533
	mov r11, rbp[-0x8]
	mov r11[0x858], eax

	mov eax, 534
	mov r11, rbp[-0x8]
	mov r11[0x85c], eax

	mov eax, 535
	mov r11, rbp[-0x8]
	mov r11[0x860], eax

	mov eax, 536
	mov r11, rbp[-0x8]
	mov r11[0x864], eax

	mov eax, 537
	mov r11, rbp[-0x8]
	mov r11[0x868], eax

	mov eax, 538
	mov r11, rbp[-0x8]
	mov r11[0x86c], eax

	mov eax, 539
	mov r11, rbp[-0x8]
	mov r11[0x870], eax

	mov eax, 540
	mov r11, rbp[-0x8]
	mov r11[0x874], eax

	mov eax, 541
	mov r11, rbp[-0x8]
	mov r11[0x878], eax

	mov eax, 542
	mov r11, rbp[-0x8]
	mov r11[0x87c], eax

	mov eax, 543
	mov r11, rbp[-0x8]
	mov r11[0x880], eax

	mov eax, 544
	mov r11, rbp[-0x8]
	mov r11[0x884], eax

	mov eax, 545
	mov r11, rbp[-0x8]
	mov r11[0x888], eax

	mov eax, 546
	mov r11, rbp[-0x8]
	mov r11[0x88c], eax

	mov eax, 547
	mov r11, rbp[-0x8]
	mov r11[0x890], eax

	mov eax, 548
	mov r11, rbp[-0x8]
	mov r11[0x894], eax

	mov eax, 549
	mov r11, rbp[-0x8]
	mov r11[0x898], eax

	mov eax, 550
	mov r11, rbp[-0x8]
	mov r11[0x89c], eax

	mov eax, 551
	mov r11, rbp[-0x8]
	mov r11[0x8a0], eax

	mov eax, 552
	mov r11, rbp[-0x8]
	mov r11[0x8a4], eax

	mov eax, 553
	mov r11, rbp[-0x8]
	mov r11[0x8a8], eax

	mov eax, 554
	mov r11, rbp[-0x8]
	mov r11[0x8ac], eax

	mov eax, 555
	mov r11, rbp[-0x8]
	mov r11[0x8b0], eax

	mov eax, 556
	mov r11, rbp[-0x8]
	mov r11[0x8b4], eax

	mov eax, 557
	mov r11, rbp[-0x8]
	mov r11[0x8b8], eax

	mov eax, 558
	mov r11, rbp[-0x8]
	mov r11[0x8bc], eax

	mov eax, 559
	mov r11, rbp[-0x8]
	mov r11[0x8c0], eax

	mov eax, 560
	mov r11, rbp[-0x8]
	mov r11[0x8c4], eax

	mov eax, 561
	mov r11, rbp[-0x8]
	mov r11[0x8c8], eax

	mov eax, 562
	mov r11, rbp[-0x8]
	mov r11[0x8cc], eax

	mov eax, 563
	mov r11, rbp[-0x8]
	mov r11[0x8d0], eax

	mov eax, 564
	mov r11, rbp[-0x8]
	mov r11[0x8d4], eax

	mov eax, 565
	mov r11, rbp[-0x8]
	mov r11[0x8d8], eax

	mov eax, 566
	mov r11, rbp[-0x8]
	mov r11[0x8dc], eax

	mov eax, 567
	mov r11, rbp[-0x8]
	mov r11[0x8e0], eax

	mov eax, 568
	mov r11, rbp[-0x8]
	mov r11[0x8e4], eax

	mov eax, 569
	mov r11, rbp[-0x8]
	mov r11[0x8e8], eax

	mov eax, 570
	mov r11, rbp[-0x8]
	mov r11[0x8ec], eax

	mov eax, 571
	mov r11, rbp[-0x8]
	mov r11[0x8f0], eax

	mov eax, 572
	mov r11, rbp[-0x8]
	mov r11[0x8f4], eax

	mov eax, 573
	mov r11, rbp[-0x8]
	mov r11[0x8f8], eax

	mov eax, 574
	mov r11, rbp[-0x8]
	mov r11[0x8fc], eax

	mov eax, 575
	mov r11, rbp[-0x8]
	mov r11[0x900], eax

	mov eax, 576
	mov r11, rbp[-0x8]
	mov r11[0x904], eax

	mov eax, 577
	mov r11, rbp[-0x8]
	mov r11[0x908], eax

	mov eax, 578
	mov r11, rbp[-0x8]
	mov r11[0x90c], eax

	mov eax, 579
	mov r11, rbp[-0x8]
	mov r11[0x910], eax

	mov eax, 580
	mov r11, rbp[-0x8]
	mov r11[0x914], eax

	mov eax, 581
	mov r11, rbp[-0x8]
	mov r11[0x918], eax

	mov eax, 582
	mov r11, rbp[-0x8]
	mov r11[0x91c], eax

	mov eax, 583
	mov r11, rbp[-0x8]
	mov r11[0x920], eax

	mov eax, 584
	mov r11, rbp[-0x8]
	mov r11[0x924], eax

	mov eax, 585
	mov r11, rbp[-0x8]
	mov r11[0x928], eax

	mov eax, 586
	mov r11, rbp[-0x8]
	mov r11[0x92c], eax

	mov eax, 587
	mov r11, rbp[-0x8]
	mov r11[0x930], eax

	mov eax, 588
	mov r11, rbp[-0x8]
	mov r11[0x934], eax

	mov eax, 589
	mov r11, rbp[-0x8]
	mov r11[0x938], eax

	mov eax, 590
	mov r11, rbp[-0x8]
	mov r11[0x93c], eax

	mov eax, 591
	mov r11, rbp[-0x8]
	mov r11[0x940], eax

	mov eax, 592
	mov r11, rbp[-0x8]
	mov r11[0x944], eax

	mov eax, 593
	mov r11, rbp[-0x8]
	mov r11[0x948], eax

	mov eax, 594
	mov r11, rbp[-0x8]
	mov r11[0x94c], eax

	mov eax, 595
	mov r11, rbp[-0x8]
	mov r11[0x950], eax

	mov eax, 596
	mov r11, rbp[-0x8]
	mov r11[0x954], eax

	mov eax, 597
	mov r11, rbp[-0x8]
	mov r11[0x958], eax

	mov eax, 598
	mov r11, rbp[-0x8]
	mov r11[0x95c], eax

	mov eax, 599
	mov r11, rbp[-0x8]
	mov r11[0x960], eax

	mov eax, 600
	mov r11, rbp[-0x8]
	mov r11[0x964], eax

	mov eax, 601
	mov r11, rbp[-0x8]
	mov r11[0x968], eax

	mov eax, 602
	mov r11, rbp[-0x8]
	mov r11[0x96c], eax

	mov eax, 603
	mov r11, rbp[-0x8]
	mov r11[0x970], eax

	mov eax, 604
	mov r11, rbp[-0x8]
	mov r11[0x974], eax

	mov eax, 605
	mov r11, rbp[-0x8]
	mov r11[0x978], eax

	mov eax, 606
	mov r11, rbp[-0x8]
	mov r11[0x97c], eax

	mov eax, 607
	mov r11, rbp[-0x8]
	mov r11[0x980], eax

	mov eax, 608
	mov r11, rbp[-0x8]
	mov r11[0x984], eax

	mov eax, 609
	mov r11, rbp[-0x8]
	mov r11[0x988], eax

	mov eax, 610
	mov r11, rbp[-0x8]
	mov r11[0x98c], eax

	mov eax, 611
	mov r11, rbp[-0x8]
	mov r11[0x990], eax

	mov eax, 612
	mov r11, rbp[-0x8]
	mov r11[0x994], eax

	mov eax, 613
	mov r11, rbp[-0x8]
	mov r11[0x998], eax

	mov eax, 614
	mov r11, rbp[-0x8]
	mov r11[0x99c], eax

	mov eax, 615
	mov r11, rbp[-0x8]
	mov r11[0x9a0], eax

	mov eax, 616
	mov r11, rbp[-0x8]
	mov r11[0x9a4], eax

	mov eax, 617
	mov r11, rbp[-0x8]
	mov r11[0x9a8], eax

	mov eax, 618
	mov r11, rbp[-0x8]
	mov r11[0x9ac], eax

	mov eax, 619
	mov r11, rbp[-0x8]
	mov r11[0x9b0], eax

	mov eax, 620
	mov r11, rbp[-0x8]
	mov r11[0x9b4], eax

	mov eax, 621
	mov r11, rbp[-0x8]
	mov r11[0x9b8], eax

	mov eax, 622
	mov r11, rbp[-0x8]
	mov r11[0x9bc], eax

	mov eax, 623
	mov r11, rbp[-0x8]
	mov r11[0x9c0], eax

	mov eax, 624
	mov r11, rbp[-0x8]
	mov r11[0x9c4], eax

	mov eax, 625
	mov r11, rbp[-0x8]
	mov r11[0x9c8], eax

	mov eax, 626
	mov r11, rbp[-0x8]
	mov r11[0x9cc], eax

	mov eax, 627
	mov r11, rbp[-0x8]
	mov r11[0x9d0], eax

	mov eax, 628
	mov r11, rbp[-0x8]
	mov r11[0x9d4], eax

	mov eax, 629
	mov r11, rbp[-0x8]
	mov r11[0x9d8], eax

	mov eax, 630
	mov r11, rbp[-0x8]
	mov r11[0x9dc], eax

	mov eax, 631
	mov r11, rbp[-0x8]
	mov r11[0x9e0], eax

	mov eax, 632
	mov r11, rbp[-0x8]
	mov r11[0x9e4], eax

	mov eax, 633
	mov r11, rbp[-0x8]
	mov r11[0x9e8], eax

	mov eax, 634
	mov r11, rbp[-0x8]
	mov r11[0x9ec], eax

	mov eax, 635
	mov r11, rbp[-0x8]
	mov r11[0x9f0], eax

	mov eax, 636
	mov r11, rbp[-0x8]
	mov r11[0x9f4], eax

	mov eax, 637
	mov r11, rbp[-0x8]
	mov r11[0x9f8], eax

	mov eax, 638
	mov r11, rbp[-0x8]
	mov r11[0x9fc], eax

	mov eax, 639
	mov r11, rbp[-0x8]
	mov r11[0xa00], eax

	mov eax, 640
	mov r11, rbp[-0x8]
	mov r11[0xa04], eax

	mov eax, 641
	mov r11, rbp[-0x8]
	mov r11[0xa08], eax

	mov eax, 642
	mov r11, rbp[-0x8]
	mov r11[0xa0c], eax

	mov eax, 643
	mov r11, rbp[-0x8]
	mov r11[0xa10], eax

	mov eax, 644
	mov r11, rbp[-0x8]
	mov r11[0xa14], eax

	mov eax, 645
	mov r11, rbp[-0x8]
	mov r11[0xa18], eax

	mov eax, 646
	mov r11, rbp[-0x8]
	mov r11[0xa1c], eax

	mov eax, 647
	mov r11, rbp[-0x8]
	mov r11[0xa20], eax

	mov eax, 648
	mov r11, rbp[-0x8]
	mov r11[0xa24], eax

	mov eax, 649
	mov r11, rbp[-0x8]
	mov r11[0xa28], eax

	mov eax, 650
	mov r11, rbp[-0x8]
	mov r11[0xa2c], eax

	mov eax, 651
	mov r11, rbp[-0x8]
	mov r11[0xa30], eax

	mov eax, 652
	mov r11, rbp[-0x8]
	mov r11[0xa34], eax

	mov eax, 653
	mov r11, rbp[-0x8]
	mov r11[0xa38], eax

	mov eax, 654
	mov r11, rbp[-0x8]
	mov r11[0xa3c], eax

	mov eax, 655
	mov r11, rbp[-0x8]
	mov r11[0xa40], eax

	mov eax, 656
	mov r11, rbp[-0x8]
	mov r11[0xa44], eax

	mov eax, 657
	mov r11, rbp[-0x8]
	mov r11[0xa48], eax

	mov eax, 658
	mov r11, rbp[-0x8]
	mov r11[0xa4c], eax

	mov eax, 659
	mov r11, rbp[-0x8]
	mov r11[0xa50], eax

	mov eax, 660
	mov r11, rbp[-0x8]
	mov r11[0xa54], eax

	mov eax, 661
	mov r11, rbp[-0x8]
	mov r11[0xa58], eax

	mov eax, 662
	mov r11, rbp[-0x8]
	mov r11[0xa5c], eax

	mov eax, 663
	mov r11, rbp[-0x8]
	mov r11[0xa60], eax

	mov eax, 664
	mov r11, rbp[-0x8]
	mov r11[0xa64], eax

	mov eax, 665
	mov r11, rbp[-0x8]
	mov r11[0xa68], eax

	mov eax, 666
	mov r11, rbp[-0x8]
	mov r11[0xa6c], eax

	mov eax, 667
	mov r11, rbp[-0x8]
	mov r11[0xa70], eax

	mov eax, 668
	mov r11, rbp[-0x8]
	mov r11[0xa74], eax

	mov eax, 669
	mov r11, rbp[-0x8]
	mov r11[0xa78], eax

	mov eax, 670
	mov r11, rbp[-0x8]
	mov r11[0xa7c], eax

	mov eax, 671
	mov r11, rbp[-0x8]
	mov r11[0xa80], eax

	mov eax, 672
	mov r11, rbp[-0x8]
	mov r11[0xa84], eax

	mov eax, 673
	mov r11, rbp[-0x8]
	mov r11[0xa88], eax

	mov eax, 674
	mov r11, rbp[-0x8]
	mov r11[0xa8c], eax

	mov eax, 675
	mov r11, rbp[-0x8]
	mov r11[0xa90], eax

	mov eax, 676
	mov r11, rbp[-0x8]
	mov r11[0xa94], eax

	mov eax, 677
	mov r11, rbp[-0x8]
	mov r11[0xa98], eax

	mov eax, 678
	mov r11, rbp[-0x8]
	mov r11[0xa9c], eax

	mov eax, 679
	mov r11, rbp[-0x8]
	mov r11[0xaa0], eax

	mov eax, 680
	mov r11, rbp[-0x8]
	mov r11[0xaa4], eax

	mov eax, 681
	mov r11, rbp[-0x8]
	mov r11[0xaa8], eax

	mov eax, 682
	mov r11, rbp[-0x8]
	mov r11[0xaac], eax

	mov eax, 683
	mov r11, rbp[-0x8]
	mov r11[0xab0], eax

	mov eax, 684
	mov r11, rbp[-0x8]
	mov r11[0xab4], eax

	mov eax, 685
	mov r11, rbp[-0x8]
	mov r11[0xab8], eax

	mov eax, 686
	mov r11, rbp[-0x8]
	mov r11[0xabc], eax

	mov eax, 687
	mov r11, rbp[-0x8]
	mov r11[0xac0], eax

	mov eax, 688
	mov r11, rbp[-0x8]
	mov r11[0xac4], eax

	mov eax, 689
	mov r11, rbp[-0x8]
	mov r11[0xac8], eax

	mov eax, 690
	mov r11, rbp[-0x8]
	mov r11[0xacc], eax

	mov eax, 691
	mov r11, rbp[-0x8]
	mov r11[0xad0], eax

	mov eax, 692
	mov r11, rbp[-0x8]
	mov r11[0xad4], eax

	mov eax, 693
	mov r11, rbp[-0x8]
	mov r11[0xad8], eax

	mov eax, 694
	mov r11, rbp[-0x8]
	mov r11[0xadc], eax

	mov eax, 695
	mov r11, rbp[-0x8]
	mov r11[0xae0], eax

	mov eax, 696
	mov r11, rbp[-0x8]
	mov r11[0xae4], eax

	mov eax, 697
	mov r11, rbp[-0x8]
	mov r11[0xae8], eax

	mov eax, 698
	mov r11, rbp[-0x8]
	mov r11[0xaec], eax

	mov eax, 699
	mov r11, rbp[-0x8]
	mov r11[0xaf0], eax

	mov eax, 700
	mov r11, rbp[-0x8]
	mov r11[0xaf4], eax

	mov eax, 701
	mov r11, rbp[-0x8]
	mov r11[0xaf8], eax

	mov eax, 702
	mov r11, rbp[-0x8]
	mov r11[0xafc], eax

	mov eax, 703
	mov r11, rbp[-0x8]
	mov r11[0xb00], eax

	mov eax, 704
	mov r11, rbp[-0x8]
	mov r11[0xb04], eax

	mov eax, 705
	mov r11, rbp[-0x8]
	mov r11[0xb08], eax

	mov eax, 706
	mov r11, rbp[-0x8]
	mov r11[0xb0c], eax

	mov eax, 707
	mov r11, rbp[-0x8]
	mov r11[0xb10], eax

	mov eax, 708
	mov r11, rbp[-0x8]
	mov r11[0xb14], eax

	mov eax, 709
	mov r11, rbp[-0x8]
	mov r11[0xb18], eax

	mov eax, 710
	mov r11, rbp[-0x8]
	mov r11[0xb1c], eax

	mov eax, 711
	mov r11, rbp[-0x8]
	mov r11[0xb20], eax

	mov eax, 712
	mov r11, rbp[-0x8]
	mov r11[0xb24], eax

	mov eax, 713
	mov r11, rbp[-0x8]
	mov r11[0xb28], eax

	mov eax, 714
	mov r11, rbp[-0x8]
	mov r11[0xb2c], eax

	mov eax, 715
	mov r11, rbp[-0x8]
	mov r11[0xb30], eax

	mov eax, 716
	mov r11, rbp[-0x8]
	mov r11[0xb34], eax

	mov eax, 717
	mov r11, rbp[-0x8]
	mov r11[0xb38], eax

	mov eax, 718
	mov r11, rbp[-0x8]
	mov r11[0xb3c], eax

	mov eax, 719
	mov r11, rbp[-0x8]
	mov r11[0xb40], eax

	mov eax, 720
	mov r11, rbp[-0x8]
	mov r11[0xb44], eax

	mov eax, 721
	mov r11, rbp[-0x8]
	mov r11[0xb48], eax

	mov eax, 722
	mov r11, rbp[-0x8]
	mov r11[0xb4c], eax

	mov eax, 723
	mov r11, rbp[-0x8]
	mov r11[0xb50], eax

	mov eax, 724
	mov r11, rbp[-0x8]
	mov r11[0xb54], eax

	mov eax, 725
	mov r11, rbp[-0x8]
	mov r11[0xb58], eax

	mov eax, 726
	mov r11, rbp[-0x8]
	mov r11[0xb5c], eax

	mov eax, 727
	mov r11, rbp[-0x8]
	mov r11[0xb60], eax

	mov eax, 728
	mov r11, rbp[-0x8]
	mov r11[0xb64], eax

	mov eax, 729
	mov r11, rbp[-0x8]
	mov r11[0xb68], eax

	mov eax, 730
	mov r11, rbp[-0x8]
	mov r11[0xb6c], eax

	mov eax, 731
	mov r11, rbp[-0x8]
	mov r11[0xb70], eax

	mov eax, 732
	mov r11, rbp[-0x8]
	mov r11[0xb74], eax

	mov eax, 733
	mov r11, rbp[-0x8]
	mov r11[0xb78], eax

	mov eax, 734
	mov r11, rbp[-0x8]
	mov r11[0xb7c], eax

	mov eax, 735
	mov r11, rbp[-0x8]
	mov r11[0xb80], eax

	mov eax, 736
	mov r11, rbp[-0x8]
	mov r11[0xb84], eax

	mov eax, 737
	mov r11, rbp[-0x8]
	mov r11[0xb88], eax

	mov eax, 738
	mov r11, rbp[-0x8]
	mov r11[0xb8c], eax

	mov eax, 739
	mov r11, rbp[-0x8]
	mov r11[0xb90], eax

	mov eax, 740
	mov r11, rbp[-0x8]
	mov r11[0xb94], eax

	mov eax, 741
	mov r11, rbp[-0x8]
	mov r11[0xb98], eax

	mov eax, 742
	mov r11, rbp[-0x8]
	mov r11[0xb9c], eax

	mov eax, 743
	mov r11, rbp[-0x8]
	mov r11[0xba0], eax

	mov eax, 744
	mov r11, rbp[-0x8]
	mov r11[0xba4], eax

	mov eax, 745
	mov r11, rbp[-0x8]
	mov r11[0xba8], eax

	mov eax, 746
	mov r11, rbp[-0x8]
	mov r11[0xbac], eax

	mov eax, 747
	mov r11, rbp[-0x8]
	mov r11[0xbb0], eax

	mov eax, 748
	mov r11, rbp[-0x8]
	mov r11[0xbb4], eax

	mov eax, 749
	mov r11, rbp[-0x8]
	mov r11[0xbb8], eax

	mov eax, 750
	mov r11, rbp[-0x8]
	mov r11[0xbbc], eax

	mov eax, 751
	mov r11, rbp[-0x8]
	mov r11[0xbc0], eax

	mov eax, 752
	mov r11, rbp[-0x8]
	mov r11[0xbc4], eax

	mov eax, 753
	mov r11, rbp[-0x8]
	mov r11[0xbc8], eax

	mov eax, 754
	mov r11, rbp[-0x8]
	mov r11[0xbcc], eax

	mov eax, 755
	mov r11, rbp[-0x8]
	mov r11[0xbd0], eax

	mov eax, 756
	mov r11, rbp[-0x8]
	mov r11[0xbd4], eax

	mov eax, 757
	mov r11, rbp[-0x8]
	mov r11[0xbd8], eax

	mov eax, 758
	mov r11, rbp[-0x8]
	mov r11[0xbdc], eax

	mov eax, 759
	mov r11, rbp[-0x8]
	mov r11[0xbe0], eax

	mov eax, 760
	mov r11, rbp[-0x8]
	mov r11[0xbe4], eax

	mov eax, 761
	mov r11, rbp[-0x8]
	mov r11[0xbe8], eax

	mov eax, 762
	mov r11, rbp[-0x8]
	mov r11[0xbec], eax

	mov eax, 763
	mov r11, rbp[-0x8]
	mov r11[0xbf0], eax

	mov eax, 764
	mov r11, rbp[-0x8]
	mov r11[0xbf4], eax

	mov eax, 765
	mov r11, rbp[-0x8]
	mov r11[0xbf8], eax

	mov eax, 766
	mov r11, rbp[-0x8]
	mov r11[0xbfc], eax

	mov eax, 767
	mov r11, rbp[-0x8]
	mov r11[0xc00], eax

	mov eax, 768
	mov r11, rbp[-0x8]
	mov r11[0xc04], eax

	mov eax, 769
	mov r11, rbp[-0x8]
	mov r11[0xc08], eax

	mov eax, 770
	mov r11, rbp[-0x8]
	mov r11[0xc0c], eax

	mov eax, 771
	mov r11, rbp[-0x8]
	mov r11[0xc10], eax

	mov eax, 772
	mov r11, rbp[-0x8]
	mov r11[0xc14], eax

	mov eax, 773
	mov r11, rbp[-0x8]
	mov r11[0xc18], eax

	mov eax, 774
	mov r11, rbp[-0x8]
	mov r11[0xc1c], eax

	mov eax, 775
	mov r11, rbp[-0x8]
	mov r11[0xc20], eax

	mov eax, 776
	mov r11, rbp[-0x8]
	mov r11[0xc24], eax

	mov eax, 777
	mov r11, rbp[-0x8]
	mov r11[0xc28], eax

	mov eax, 778
	mov r11, rbp[-0x8]
	mov r11[0xc2c], eax

	mov eax, 779
	mov r11, rbp[-0x8]
	mov r11[0xc30], eax

	mov eax, 780
	mov r11, rbp[-0x8]
	mov r11[0xc34], eax

	mov eax, 781
	mov r11, rbp[-0x8]
	mov r11[0xc38], eax

	mov eax, 782
	mov r11, rbp[-0x8]
	mov r11[0xc3c], eax

	mov eax, 783
	mov r11, rbp[-0x8]
	mov r11[0xc40], eax

	mov eax, 784
	mov r11, rbp[-0x8]
	mov r11[0xc44], eax

	mov eax, 785
	mov r11, rbp[-0x8]
	mov r11[0xc48], eax

	mov eax, 786
	mov r11, rbp[-0x8]
	mov r11[0xc4c], eax

	mov eax, 787
	mov r11, rbp[-0x8]
	mov r11[0xc50], eax

	mov eax, 788
	mov r11, rbp[-0x8]
	mov r11[0xc54], eax

	mov eax, 789
	mov r11, rbp[-0x8]
	mov r11[0xc58], eax

	mov eax, 790
	mov r11, rbp[-0x8]
	mov r11[0xc5c], eax

	mov eax, 791
	mov r11, rbp[-0x8]
	mov r11[0xc60], eax

	mov eax, 792
	mov r11, rbp[-0x8]
	mov r11[0xc64], eax

	mov eax, 793
	mov r11, rbp[-0x8]
	mov r11[0xc68], eax

	mov eax, 794
	mov r11, rbp[-0x8]
	mov r11[0xc6c], eax

	mov eax, 795
	mov r11, rbp[-0x8]
	mov r11[0xc70], eax

	mov eax, 796
	mov r11, rbp[-0x8]
	mov r11[0xc74], eax

	mov eax, 797
	mov r11, rbp[-0x8]
	mov r11[0xc78], eax

	mov eax, 798
	mov r11, rbp[-0x8]
	mov r11[0xc7c], eax

	mov eax, 799
	mov r11, rbp[-0x8]
	mov r11[0xc80], eax

	mov eax, 800
	mov r11, rbp[-0x8]
	mov r11[0xc84], eax

	mov eax, 801
	mov r11, rbp[-0x8]
	mov r11[0xc88], eax

	mov eax, 802
	mov r11, rbp[-0x8]
	mov r11[0xc8c], eax

	mov eax, 803
	mov r11, rbp[-0x8]
	mov r11[0xc90], eax

	mov eax, 804
	mov r11, rbp[-0x8]
	mov r11[0xc94], eax

	mov eax, 805
	mov r11, rbp[-0x8]
	mov r11[0xc98], eax

	mov eax, 806
	mov r11, rbp[-0x8]
	mov r11[0xc9c], eax

	mov eax, 807
	mov r11, rbp[-0x8]
	mov r11[0xca0], eax

	mov eax, 808
	mov r11, rbp[-0x8]
	mov r11[0xca4], eax

	mov eax, 809
	mov r11, rbp[-0x8]
	mov r11[0xca8], eax

	mov eax, 810
	mov r11, rbp[-0x8]
	mov r11[0xcac], eax

	mov eax, 811
	mov r11, rbp[-0x8]
	mov r11[0xcb0], eax

	mov eax, 812
	mov r11, rbp[-0x8]
	mov r11[0xcb4], eax

	mov eax, 813
	mov r11, rbp[-0x8]
	mov r11[0xcb8], eax

	mov eax, 814
	mov r11, rbp[-0x8]
	mov r11[0xcbc], eax

	mov eax, 815
	mov r11, rbp[-0x8]
	mov r11[0xcc0], eax

	mov eax, 816
	mov r11, rbp[-0x8]
	mov r11[0xcc4], eax

	mov eax, 817
	mov r11, rbp[-0x8]
	mov r11[0xcc8], eax

	mov eax, 818
	mov r11, rbp[-0x8]
	mov r11[0xccc], eax

	mov eax, 819
	mov r11, rbp[-0x8]
	mov r11[0xcd0], eax

	mov eax, 820
	mov r11, rbp[-0x8]
	mov r11[0xcd4], eax

	mov eax, 821
	mov r11, rbp[-0x8]
	mov r11[0xcd8], eax

	mov eax, 822
	mov r11, rbp[-0x8]
	mov r11[0xcdc], eax

	mov eax, 823
	mov r11, rbp[-0x8]
	mov r11[0xce0], eax

	mov eax, 824
	mov r11, rbp[-0x8]
	mov r11[0xce4], eax

	mov eax, 825
	mov r11, rbp[-0x8]
	mov r11[0xce8], eax

	mov eax, 826
	mov r11, rbp[-0x8]
	mov r11[0xcec], eax

	mov eax, 827
	mov r11, rbp[-0x8]
	mov r11[0xcf0], eax

	mov eax, 828
	mov r11, rbp[-0x8]
	mov r11[0xcf4], eax

	mov eax, 829
	mov r11, rbp[-0x8]
	mov r11[0xcf8], eax

	mov eax, 830
	mov r11, rbp[-0x8]
	mov r11[0xcfc], eax

	mov eax, 831
	mov r11, rbp[-0x8]
	mov r11[0xd00], eax

	mov eax, 832
	mov r11, rbp[-0x8]
	mov r11[0xd04], eax

	mov eax, 833
	mov r11, rbp[-0x8]
	mov r11[0xd08], eax

	mov eax, 834
	mov r11, rbp[-0x8]
	mov r11[0xd0c], eax

	mov eax, 835
	mov r11, rbp[-0x8]
	mov r11[0xd10], eax

	mov eax, 836
	mov r11, rbp[-0x8]
	mov r11[0xd14], eax

	mov eax, 837
	mov r11, rbp[-0x8]
	mov r11[0xd18], eax

	mov eax, 838
	mov r11, rbp[-0x8]
	mov r11[0xd1c], eax

	mov eax, 839
	mov r11, rbp[-0x8]
	mov r11[0xd20], eax

	mov eax, 840
	mov r11, rbp[-0x8]
	mov r11[0xd24], eax

	mov eax, 841
	mov r11, rbp[-0x8]
	mov r11[0xd28], eax

	mov eax, 842
	mov r11, rbp[-0x8]
	mov r11[0xd2c], eax

	mov eax, 843
	mov r11, rbp[-0x8]
	mov r11[0xd30], eax

	mov eax, 844
	mov r11, rbp[-0x8]
	mov r11[0xd34], eax

	mov eax, 845
	mov r11, rbp[-0x8]
	mov r11[0xd38], eax

	mov eax, 846
	mov r11, rbp[-0x8]
	mov r11[0xd3c], eax

	mov eax, 847
	mov r11, rbp[-0x8]
	mov r11[0xd40], eax

	mov eax, 848
	mov r11, rbp[-0x8]
	mov r11[0xd44], eax

	mov eax, 849
	mov r11, rbp[-0x8]
	mov r11[0xd48], eax

	mov eax, 850
	mov r11, rbp[-0x8]
	mov r11[0xd4c], eax

	mov eax, 851
	mov r11, rbp[-0x8]
	mov r11[0xd50], eax

	mov eax, 852
	mov r11, rbp[-0x8]
	mov r11[0xd54], eax

	mov eax, 853
	mov r11, rbp[-0x8]
	mov r11[0xd58], eax

	mov eax, 854
	mov r11, rbp[-0x8]
	mov r11[0xd5c], eax

	mov eax, 855
	mov r11, rbp[-0x8]
	mov r11[0xd60], eax

	mov eax, 856
	mov r11, rbp[-0x8]
	mov r11[0xd64], eax

	mov eax, 857
	mov r11, rbp[-0x8]
	mov r11[0xd68], eax

	mov eax, 858
	mov r11, rbp[-0x8]
	mov r11[0xd6c], eax

	mov eax, 859
	mov r11, rbp[-0x8]
	mov r11[0xd70], eax

	mov eax, 860
	mov r11, rbp[-0x8]
	mov r11[0xd74], eax

	mov eax, 861
	mov r11, rbp[-0x8]
	mov r11[0xd78], eax

	mov eax, 862
	mov r11, rbp[-0x8]
	mov r11[0xd7c], eax

	mov eax, 863
	mov r11, rbp[-0x8]
	mov r11[0xd80], eax

	mov eax, 864
	mov r11, rbp[-0x8]
	mov r11[0xd84], eax

	mov eax, 865
	mov r11, rbp[-0x8]
	mov r11[0xd88], eax

	mov eax, 866
	mov r11, rbp[-0x8]
	mov r11[0xd8c], eax

	mov eax, 867
	mov r11, rbp[-0x8]
	mov r11[0xd90], eax

	mov eax, 868
	mov r11, rbp[-0x8]
	mov r11[0xd94], eax

	mov eax, 869
	mov r11, rbp[-0x8]
	mov r11[0xd98], eax

	mov eax, 870
	mov r11, rbp[-0x8]
	mov r11[0xd9c], eax

	mov eax, 871
	mov r11, rbp[-0x8]
	mov r11[0xda0], eax

	mov eax, 872
	mov r11, rbp[-0x8]
	mov r11[0xda4], eax

	mov eax, 873
	mov r11, rbp[-0x8]
	mov r11[0xda8], eax

	mov eax, 874
	mov r11, rbp[-0x8]
	mov r11[0xdac], eax

	mov eax, 875
	mov r11, rbp[-0x8]
	mov r11[0xdb0], eax

	mov eax, 876
	mov r11, rbp[-0x8]
	mov r11[0xdb4], eax

	mov eax, 877
	mov r11, rbp[-0x8]
	mov r11[0xdb8], eax

	mov eax, 878
	mov r11, rbp[-0x8]
	mov r11[0xdbc], eax

	mov eax, 879
	mov r11, rbp[-0x8]
	mov r11[0xdc0], eax

	mov eax, 880
	mov r11, rbp[-0x8]
	mov r11[0xdc4], eax

	mov eax, 881
	mov r11, rbp[-0x8]
	mov r11[0xdc8], eax

	mov eax, 882
	mov r11, rbp[-0x8]
	mov r11[0xdcc], eax

	mov eax, 883
	mov r11, rbp[-0x8]
	mov r11[0xdd0], eax

	mov eax, 884
	mov r11, rbp[-0x8]
	mov r11[0xdd4], eax

	mov eax, 885
	mov r11, rbp[-0x8]
	mov r11[0xdd8], eax

	mov eax, 886
	mov r11, rbp[-0x8]
	mov r11[0xddc], eax

	mov eax, 887
	mov r11, rbp[-0x8]
	mov r11[0xde0], eax

	mov eax, 888
	mov r11, rbp[-0x8]
	mov r11[0xde4], eax

	mov eax, 889
	mov r11, rbp[-0x8]
	mov r11[0xde8], eax

	mov eax, 890
	mov r11, rbp[-0x8]
	mov r11[0xdec], eax

	mov eax, 891
	mov r11, rbp[-0x8]
	mov r11[0xdf0], eax

	mov eax, 892
	mov r11, rbp[-0x8]
	mov r11[0xdf4], eax

	mov eax, 893
	mov r11, rbp[-0x8]
	mov r11[0xdf8], eax

	mov eax, 894
	mov r11, rbp[-0x8]
	mov r11[0xdfc], eax

	mov eax, 895
	mov r11, rbp[-0x8]
	mov r11[0xe00], eax

	mov eax, 896
	mov r11, rbp[-0x8]
	mov r11[0xe04], eax

	mov eax, 897
	mov r11, rbp[-0x8]
	mov r11[0xe08], eax

	mov eax, 898
	mov r11, rbp[-0x8]
	mov r11[0xe0c], eax

	mov eax, 899
	mov r11, rbp[-0x8]
	mov r11[0xe10], eax

	mov eax, 900
	mov r11, rbp[-0x8]
	mov r11[0xe14], eax

	mov eax, 901
	mov r11, rbp[-0x8]
	mov r11[0xe18], eax

	mov eax, 902
	mov r11, rbp[-0x8]
	mov r11[0xe1c], eax

	mov eax, 903
	mov r11, rbp[-0x8]
	mov r11[0xe20], eax

	mov eax, 904
	mov r11, rbp[-0x8]
	mov r11[0xe24], eax

	mov eax, 905
	mov r11, rbp[-0x8]
	mov r11[0xe28], eax

	mov eax, 906
	mov r11, rbp[-0x8]
	mov r11[0xe2c], eax

	mov eax, 907
	mov r11, rbp[-0x8]
	mov r11[0xe30], eax

	mov eax, 908
	mov r11, rbp[-0x8]
	mov r11[0xe34], eax

	mov eax, 909
	mov r11, rbp[-0x8]
	mov r11[0xe38], eax

	mov eax, 910
	mov r11, rbp[-0x8]
	mov r11[0xe3c], eax

	mov eax, 911
	mov r11, rbp[-0x8]
	mov r11[0xe40], eax

	mov eax, 912
	mov r11, rbp[-0x8]
	mov r11[0xe44], eax

	mov eax, 913
	mov r11, rbp[-0x8]
	mov r11[0xe48], eax

	mov eax, 914
	mov r11, rbp[-0x8]
	mov r11[0xe4c], eax

	mov eax, 915
	mov r11, rbp[-0x8]
	mov r11[0xe50], eax

	mov eax, 916
	mov r11, rbp[-0x8]
	mov r11[0xe54], eax

	mov eax, 917
	mov r11, rbp[-0x8]
	mov r11[0xe58], eax

	mov eax, 918
	mov r11, rbp[-0x8]
	mov r11[0xe5c], eax

	mov eax, 919
	mov r11, rbp[-0x8]
	mov r11[0xe60], eax

	mov eax, 920
	mov r11, rbp[-0x8]
	mov r11[0xe64], eax

	mov eax, 921
	mov r11, rbp[-0x8]
	mov r11[0xe68], eax

	mov eax, 922
	mov r11, rbp[-0x8]
	mov r11[0xe6c], eax

	mov eax, 923
	mov r11, rbp[-0x8]
	mov r11[0xe70], eax

	mov eax, 924
	mov r11, rbp[-0x8]
	mov r11[0xe74], eax

	mov eax, 925
	mov r11, rbp[-0x8]
	mov r11[0xe78], eax

	mov eax, 926
	mov r11, rbp[-0x8]
	mov r11[0xe7c], eax

	mov eax, 927
	mov r11, rbp[-0x8]
	mov r11[0xe80], eax

	mov eax, 928
	mov r11, rbp[-0x8]
	mov r11[0xe84], eax

	mov eax, 929
	mov r11, rbp[-0x8]
	mov r11[0xe88], eax

	mov eax, 930
	mov r11, rbp[-0x8]
	mov r11[0xe8c], eax

	mov eax, 931
	mov r11, rbp[-0x8]
	mov r11[0xe90], eax

	mov eax, 932
	mov r11, rbp[-0x8]
	mov r11[0xe94], eax

	mov eax, 933
	mov r11, rbp[-0x8]
	mov r11[0xe98], eax

	mov eax, 934
	mov r11, rbp[-0x8]
	mov r11[0xe9c], eax

	mov eax, 935
	mov r11, rbp[-0x8]
	mov r11[0xea0], eax

	mov eax, 936
	mov r11, rbp[-0x8]
	mov r11[0xea4], eax

	mov eax, 937
	mov r11, rbp[-0x8]
	mov r11[0xea8], eax

	mov eax, 938
	mov r11, rbp[-0x8]
	mov r11[0xeac], eax

	mov eax, 939
	mov r11, rbp[-0x8]
	mov r11[0xeb0], eax

	mov eax, 940
	mov r11, rbp[-0x8]
	mov r11[0xeb4], eax

	mov eax, 941
	mov r11, rbp[-0x8]
	mov r11[0xeb8], eax

	mov eax, 942
	mov r11, rbp[-0x8]
	mov r11[0xebc], eax

	mov eax, 943
	mov r11, rbp[-0x8]
	mov r11[0xec0], eax

	mov eax, 944
	mov r11, rbp[-0x8]
	mov r11[0xec4], eax

	mov eax, 945
	mov r11, rbp[-0x8]
	mov r11[0xec8], eax

	mov eax, 946
	mov r11, rbp[-0x8]
	mov r11[0xecc], eax

	mov eax, 947
	mov r11, rbp[-0x8]
	mov r11[0xed0], eax

	mov eax, 948
	mov r11, rbp[-0x8]
	mov r11[0xed4], eax

	mov eax, 949
	mov r11, rbp[-0x8]
	mov r11[0xed8], eax

	mov eax, 950
	mov r11, rbp[-0x8]
	mov r11[0xedc], eax

	mov eax, 951
	mov r11, rbp[-0x8]
	mov r11[0xee0], eax

	mov eax, 952
	mov r11, rbp[-0x8]
	mov r11[0xee4], eax

	mov eax, 953
	mov r11, rbp[-0x8]
	mov r11[0xee8], eax

	mov eax, 954
	mov r11, rbp[-0x8]
	mov r11[0xeec], eax

	mov eax, 955
	mov r11, rbp[-0x8]
	mov r11[0xef0], eax

	mov eax, 956
	mov r11, rbp[-0x8]
	mov r11[0xef4], eax

	mov eax, 957
	mov r11, rbp[-0x8]
	mov r11[0xef8], eax

	mov eax, 958
	mov r11, rbp[-0x8]
	mov r11[0xefc], eax

	mov eax, 959
	mov r11, rbp[-0x8]
	mov r11[0xf00], eax

	mov eax, 960
	mov r11, rbp[-0x8]
	mov r11[0xf04], eax

	mov eax, 961
	mov r11, rbp[-0x8]
	mov r11[0xf08], eax

	mov eax, 962
	mov r11, rbp[-0x8]
	mov r11[0xf0c], eax

	mov eax, 963
	mov r11, rbp[-0x8]
	mov r11[0xf10], eax

	mov eax, 964
	mov r11, rbp[-0x8]
	mov r11[0xf14], eax

	mov eax, 965
	mov r11, rbp[-0x8]
	mov r11[0xf18], eax

	mov eax, 966
	mov r11, rbp[-0x8]
	mov r11[0xf1c], eax

	mov eax, 967
	mov r11, rbp[-0x8]
	mov r11[0xf20], eax

	mov eax, 968
	mov r11, rbp[-0x8]
	mov r11[0xf24], eax

	mov eax, 969
	mov r11, rbp[-0x8]
	mov r11[0xf28], eax

	mov eax, 970
	mov r11, rbp[-0x8]
	mov r11[0xf2c], eax

	mov eax, 971
	mov r11, rbp[-0x8]
	mov r11[0xf30], eax

	mov eax, 972
	mov r11, rbp[-0x8]
	mov r11[0xf34], eax

	mov eax, 973
	mov r11, rbp[-0x8]
	mov r11[0xf38], eax

	mov eax, 974
	mov r11, rbp[-0x8]
	mov r11[0xf3c], eax

	mov eax, 975
	mov r11, rbp[-0x8]
	mov r11[0xf40], eax

	mov eax, 976
	mov r11, rbp[-0x8]
	mov r11[0xf44], eax

	mov eax, 977
	mov r11, rbp[-0x8]
	mov r11[0xf48], eax

	mov eax, 978
	mov r11, rbp[-0x8]
	mov r11[0xf4c], eax

	mov eax, 979
	mov r11, rbp[-0x8]
	mov r11[0xf50], eax

	mov eax, 980
	mov r11, rbp[-0x8]
	mov r11[0xf54], eax

	mov eax, 981
	mov r11, rbp[-0x8]
	mov r11[0xf58], eax

	mov eax, 982
	mov r11, rbp[-0x8]
	mov r11[0xf5c], eax

	mov eax, 983
	mov r11, rbp[-0x8]
	mov r11[0xf60], eax

	mov eax, 984
	mov r11, rbp[-0x8]
	mov r11[0xf64], eax

	mov eax, 985
	mov r11, rbp[-0x8]
	mov r11[0xf68], eax

	mov eax, 986
	mov r11, rbp[-0x8]
	mov r11[0xf6c], eax

	mov eax, 987
	mov r11, rbp[-0x8]
	mov r11[0xf70], eax

	mov eax, 988
	mov r11, rbp[-0x8]
	mov r11[0xf74], eax

	mov eax, 989
	mov r11, rbp[-0x8]
	mov r11[0xf78], eax

	mov eax, 990
	mov r11, rbp[-0x8]
	mov r11[0xf7c], eax

	mov eax, 991
	mov r11, rbp[-0x8]
	mov r11[0xf80], eax

	mov eax, 992
	mov r11, rbp[-0x8]
	mov r11[0xf84], eax

	mov eax, 993
	mov r11, rbp[-0x8]
	mov r11[0xf88], eax

	mov eax, 994
	mov r11, rbp[-0x8]
	mov r11[0xf8c], eax

	mov eax, 995
	mov r11, rbp[-0x8]
	mov r11[0xf90], eax

	mov eax, 996
	mov r11, rbp[-0x8]
	mov r11[0xf94], eax

	mov eax, 997
	mov r11, rbp[-0x8]
	mov r11[0xf98], eax

	mov eax, 998
	mov r11, rbp[-0x8]
	mov r11[0xf9c], eax

	mov eax, 999
	mov r11, rbp[-0x8]
	mov r11[0xfa0], eax

	mov eax, 1000
	mov r11, rbp[-0x8]
	mov r11[0xfa4], eax

	mov rsp, rbp
	pop rbp
	ret
