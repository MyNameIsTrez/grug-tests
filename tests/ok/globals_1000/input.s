section .data

global define_type
define_type: db "a", 0

align 8
global globals_size
globals_size: dq 4000

global strings
strings:

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

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
	mov eax, 65
	mov rdi[0x100], eax
	mov eax, 66
	mov rdi[0x104], eax
	mov eax, 67
	mov rdi[0x108], eax
	mov eax, 68
	mov rdi[0x10c], eax
	mov eax, 69
	mov rdi[0x110], eax
	mov eax, 70
	mov rdi[0x114], eax
	mov eax, 71
	mov rdi[0x118], eax
	mov eax, 72
	mov rdi[0x11c], eax
	mov eax, 73
	mov rdi[0x120], eax
	mov eax, 74
	mov rdi[0x124], eax
	mov eax, 75
	mov rdi[0x128], eax
	mov eax, 76
	mov rdi[0x12c], eax
	mov eax, 77
	mov rdi[0x130], eax
	mov eax, 78
	mov rdi[0x134], eax
	mov eax, 79
	mov rdi[0x138], eax
	mov eax, 80
	mov rdi[0x13c], eax
	mov eax, 81
	mov rdi[0x140], eax
	mov eax, 82
	mov rdi[0x144], eax
	mov eax, 83
	mov rdi[0x148], eax
	mov eax, 84
	mov rdi[0x14c], eax
	mov eax, 85
	mov rdi[0x150], eax
	mov eax, 86
	mov rdi[0x154], eax
	mov eax, 87
	mov rdi[0x158], eax
	mov eax, 88
	mov rdi[0x15c], eax
	mov eax, 89
	mov rdi[0x160], eax
	mov eax, 90
	mov rdi[0x164], eax
	mov eax, 91
	mov rdi[0x168], eax
	mov eax, 92
	mov rdi[0x16c], eax
	mov eax, 93
	mov rdi[0x170], eax
	mov eax, 94
	mov rdi[0x174], eax
	mov eax, 95
	mov rdi[0x178], eax
	mov eax, 96
	mov rdi[0x17c], eax
	mov eax, 97
	mov rdi[0x180], eax
	mov eax, 98
	mov rdi[0x184], eax
	mov eax, 99
	mov rdi[0x188], eax
	mov eax, 100
	mov rdi[0x18c], eax
	mov eax, 101
	mov rdi[0x190], eax
	mov eax, 102
	mov rdi[0x194], eax
	mov eax, 103
	mov rdi[0x198], eax
	mov eax, 104
	mov rdi[0x19c], eax
	mov eax, 105
	mov rdi[0x1a0], eax
	mov eax, 106
	mov rdi[0x1a4], eax
	mov eax, 107
	mov rdi[0x1a8], eax
	mov eax, 108
	mov rdi[0x1ac], eax
	mov eax, 109
	mov rdi[0x1b0], eax
	mov eax, 110
	mov rdi[0x1b4], eax
	mov eax, 111
	mov rdi[0x1b8], eax
	mov eax, 112
	mov rdi[0x1bc], eax
	mov eax, 113
	mov rdi[0x1c0], eax
	mov eax, 114
	mov rdi[0x1c4], eax
	mov eax, 115
	mov rdi[0x1c8], eax
	mov eax, 116
	mov rdi[0x1cc], eax
	mov eax, 117
	mov rdi[0x1d0], eax
	mov eax, 118
	mov rdi[0x1d4], eax
	mov eax, 119
	mov rdi[0x1d8], eax
	mov eax, 120
	mov rdi[0x1dc], eax
	mov eax, 121
	mov rdi[0x1e0], eax
	mov eax, 122
	mov rdi[0x1e4], eax
	mov eax, 123
	mov rdi[0x1e8], eax
	mov eax, 124
	mov rdi[0x1ec], eax
	mov eax, 125
	mov rdi[0x1f0], eax
	mov eax, 126
	mov rdi[0x1f4], eax
	mov eax, 127
	mov rdi[0x1f8], eax
	mov eax, 128
	mov rdi[0x1fc], eax
	mov eax, 129
	mov rdi[0x200], eax
	mov eax, 130
	mov rdi[0x204], eax
	mov eax, 131
	mov rdi[0x208], eax
	mov eax, 132
	mov rdi[0x20c], eax
	mov eax, 133
	mov rdi[0x210], eax
	mov eax, 134
	mov rdi[0x214], eax
	mov eax, 135
	mov rdi[0x218], eax
	mov eax, 136
	mov rdi[0x21c], eax
	mov eax, 137
	mov rdi[0x220], eax
	mov eax, 138
	mov rdi[0x224], eax
	mov eax, 139
	mov rdi[0x228], eax
	mov eax, 140
	mov rdi[0x22c], eax
	mov eax, 141
	mov rdi[0x230], eax
	mov eax, 142
	mov rdi[0x234], eax
	mov eax, 143
	mov rdi[0x238], eax
	mov eax, 144
	mov rdi[0x23c], eax
	mov eax, 145
	mov rdi[0x240], eax
	mov eax, 146
	mov rdi[0x244], eax
	mov eax, 147
	mov rdi[0x248], eax
	mov eax, 148
	mov rdi[0x24c], eax
	mov eax, 149
	mov rdi[0x250], eax
	mov eax, 150
	mov rdi[0x254], eax
	mov eax, 151
	mov rdi[0x258], eax
	mov eax, 152
	mov rdi[0x25c], eax
	mov eax, 153
	mov rdi[0x260], eax
	mov eax, 154
	mov rdi[0x264], eax
	mov eax, 155
	mov rdi[0x268], eax
	mov eax, 156
	mov rdi[0x26c], eax
	mov eax, 157
	mov rdi[0x270], eax
	mov eax, 158
	mov rdi[0x274], eax
	mov eax, 159
	mov rdi[0x278], eax
	mov eax, 160
	mov rdi[0x27c], eax
	mov eax, 161
	mov rdi[0x280], eax
	mov eax, 162
	mov rdi[0x284], eax
	mov eax, 163
	mov rdi[0x288], eax
	mov eax, 164
	mov rdi[0x28c], eax
	mov eax, 165
	mov rdi[0x290], eax
	mov eax, 166
	mov rdi[0x294], eax
	mov eax, 167
	mov rdi[0x298], eax
	mov eax, 168
	mov rdi[0x29c], eax
	mov eax, 169
	mov rdi[0x2a0], eax
	mov eax, 170
	mov rdi[0x2a4], eax
	mov eax, 171
	mov rdi[0x2a8], eax
	mov eax, 172
	mov rdi[0x2ac], eax
	mov eax, 173
	mov rdi[0x2b0], eax
	mov eax, 174
	mov rdi[0x2b4], eax
	mov eax, 175
	mov rdi[0x2b8], eax
	mov eax, 176
	mov rdi[0x2bc], eax
	mov eax, 177
	mov rdi[0x2c0], eax
	mov eax, 178
	mov rdi[0x2c4], eax
	mov eax, 179
	mov rdi[0x2c8], eax
	mov eax, 180
	mov rdi[0x2cc], eax
	mov eax, 181
	mov rdi[0x2d0], eax
	mov eax, 182
	mov rdi[0x2d4], eax
	mov eax, 183
	mov rdi[0x2d8], eax
	mov eax, 184
	mov rdi[0x2dc], eax
	mov eax, 185
	mov rdi[0x2e0], eax
	mov eax, 186
	mov rdi[0x2e4], eax
	mov eax, 187
	mov rdi[0x2e8], eax
	mov eax, 188
	mov rdi[0x2ec], eax
	mov eax, 189
	mov rdi[0x2f0], eax
	mov eax, 190
	mov rdi[0x2f4], eax
	mov eax, 191
	mov rdi[0x2f8], eax
	mov eax, 192
	mov rdi[0x2fc], eax
	mov eax, 193
	mov rdi[0x300], eax
	mov eax, 194
	mov rdi[0x304], eax
	mov eax, 195
	mov rdi[0x308], eax
	mov eax, 196
	mov rdi[0x30c], eax
	mov eax, 197
	mov rdi[0x310], eax
	mov eax, 198
	mov rdi[0x314], eax
	mov eax, 199
	mov rdi[0x318], eax
	mov eax, 200
	mov rdi[0x31c], eax
	mov eax, 201
	mov rdi[0x320], eax
	mov eax, 202
	mov rdi[0x324], eax
	mov eax, 203
	mov rdi[0x328], eax
	mov eax, 204
	mov rdi[0x32c], eax
	mov eax, 205
	mov rdi[0x330], eax
	mov eax, 206
	mov rdi[0x334], eax
	mov eax, 207
	mov rdi[0x338], eax
	mov eax, 208
	mov rdi[0x33c], eax
	mov eax, 209
	mov rdi[0x340], eax
	mov eax, 210
	mov rdi[0x344], eax
	mov eax, 211
	mov rdi[0x348], eax
	mov eax, 212
	mov rdi[0x34c], eax
	mov eax, 213
	mov rdi[0x350], eax
	mov eax, 214
	mov rdi[0x354], eax
	mov eax, 215
	mov rdi[0x358], eax
	mov eax, 216
	mov rdi[0x35c], eax
	mov eax, 217
	mov rdi[0x360], eax
	mov eax, 218
	mov rdi[0x364], eax
	mov eax, 219
	mov rdi[0x368], eax
	mov eax, 220
	mov rdi[0x36c], eax
	mov eax, 221
	mov rdi[0x370], eax
	mov eax, 222
	mov rdi[0x374], eax
	mov eax, 223
	mov rdi[0x378], eax
	mov eax, 224
	mov rdi[0x37c], eax
	mov eax, 225
	mov rdi[0x380], eax
	mov eax, 226
	mov rdi[0x384], eax
	mov eax, 227
	mov rdi[0x388], eax
	mov eax, 228
	mov rdi[0x38c], eax
	mov eax, 229
	mov rdi[0x390], eax
	mov eax, 230
	mov rdi[0x394], eax
	mov eax, 231
	mov rdi[0x398], eax
	mov eax, 232
	mov rdi[0x39c], eax
	mov eax, 233
	mov rdi[0x3a0], eax
	mov eax, 234
	mov rdi[0x3a4], eax
	mov eax, 235
	mov rdi[0x3a8], eax
	mov eax, 236
	mov rdi[0x3ac], eax
	mov eax, 237
	mov rdi[0x3b0], eax
	mov eax, 238
	mov rdi[0x3b4], eax
	mov eax, 239
	mov rdi[0x3b8], eax
	mov eax, 240
	mov rdi[0x3bc], eax
	mov eax, 241
	mov rdi[0x3c0], eax
	mov eax, 242
	mov rdi[0x3c4], eax
	mov eax, 243
	mov rdi[0x3c8], eax
	mov eax, 244
	mov rdi[0x3cc], eax
	mov eax, 245
	mov rdi[0x3d0], eax
	mov eax, 246
	mov rdi[0x3d4], eax
	mov eax, 247
	mov rdi[0x3d8], eax
	mov eax, 248
	mov rdi[0x3dc], eax
	mov eax, 249
	mov rdi[0x3e0], eax
	mov eax, 250
	mov rdi[0x3e4], eax
	mov eax, 251
	mov rdi[0x3e8], eax
	mov eax, 252
	mov rdi[0x3ec], eax
	mov eax, 253
	mov rdi[0x3f0], eax
	mov eax, 254
	mov rdi[0x3f4], eax
	mov eax, 255
	mov rdi[0x3f8], eax
	mov eax, 256
	mov rdi[0x3fc], eax
	mov eax, 257
	mov rdi[0x400], eax
	mov eax, 258
	mov rdi[0x404], eax
	mov eax, 259
	mov rdi[0x408], eax
	mov eax, 260
	mov rdi[0x40c], eax
	mov eax, 261
	mov rdi[0x410], eax
	mov eax, 262
	mov rdi[0x414], eax
	mov eax, 263
	mov rdi[0x418], eax
	mov eax, 264
	mov rdi[0x41c], eax
	mov eax, 265
	mov rdi[0x420], eax
	mov eax, 266
	mov rdi[0x424], eax
	mov eax, 267
	mov rdi[0x428], eax
	mov eax, 268
	mov rdi[0x42c], eax
	mov eax, 269
	mov rdi[0x430], eax
	mov eax, 270
	mov rdi[0x434], eax
	mov eax, 271
	mov rdi[0x438], eax
	mov eax, 272
	mov rdi[0x43c], eax
	mov eax, 273
	mov rdi[0x440], eax
	mov eax, 274
	mov rdi[0x444], eax
	mov eax, 275
	mov rdi[0x448], eax
	mov eax, 276
	mov rdi[0x44c], eax
	mov eax, 277
	mov rdi[0x450], eax
	mov eax, 278
	mov rdi[0x454], eax
	mov eax, 279
	mov rdi[0x458], eax
	mov eax, 280
	mov rdi[0x45c], eax
	mov eax, 281
	mov rdi[0x460], eax
	mov eax, 282
	mov rdi[0x464], eax
	mov eax, 283
	mov rdi[0x468], eax
	mov eax, 284
	mov rdi[0x46c], eax
	mov eax, 285
	mov rdi[0x470], eax
	mov eax, 286
	mov rdi[0x474], eax
	mov eax, 287
	mov rdi[0x478], eax
	mov eax, 288
	mov rdi[0x47c], eax
	mov eax, 289
	mov rdi[0x480], eax
	mov eax, 290
	mov rdi[0x484], eax
	mov eax, 291
	mov rdi[0x488], eax
	mov eax, 292
	mov rdi[0x48c], eax
	mov eax, 293
	mov rdi[0x490], eax
	mov eax, 294
	mov rdi[0x494], eax
	mov eax, 295
	mov rdi[0x498], eax
	mov eax, 296
	mov rdi[0x49c], eax
	mov eax, 297
	mov rdi[0x4a0], eax
	mov eax, 298
	mov rdi[0x4a4], eax
	mov eax, 299
	mov rdi[0x4a8], eax
	mov eax, 300
	mov rdi[0x4ac], eax
	mov eax, 301
	mov rdi[0x4b0], eax
	mov eax, 302
	mov rdi[0x4b4], eax
	mov eax, 303
	mov rdi[0x4b8], eax
	mov eax, 304
	mov rdi[0x4bc], eax
	mov eax, 305
	mov rdi[0x4c0], eax
	mov eax, 306
	mov rdi[0x4c4], eax
	mov eax, 307
	mov rdi[0x4c8], eax
	mov eax, 308
	mov rdi[0x4cc], eax
	mov eax, 309
	mov rdi[0x4d0], eax
	mov eax, 310
	mov rdi[0x4d4], eax
	mov eax, 311
	mov rdi[0x4d8], eax
	mov eax, 312
	mov rdi[0x4dc], eax
	mov eax, 313
	mov rdi[0x4e0], eax
	mov eax, 314
	mov rdi[0x4e4], eax
	mov eax, 315
	mov rdi[0x4e8], eax
	mov eax, 316
	mov rdi[0x4ec], eax
	mov eax, 317
	mov rdi[0x4f0], eax
	mov eax, 318
	mov rdi[0x4f4], eax
	mov eax, 319
	mov rdi[0x4f8], eax
	mov eax, 320
	mov rdi[0x4fc], eax
	mov eax, 321
	mov rdi[0x500], eax
	mov eax, 322
	mov rdi[0x504], eax
	mov eax, 323
	mov rdi[0x508], eax
	mov eax, 324
	mov rdi[0x50c], eax
	mov eax, 325
	mov rdi[0x510], eax
	mov eax, 326
	mov rdi[0x514], eax
	mov eax, 327
	mov rdi[0x518], eax
	mov eax, 328
	mov rdi[0x51c], eax
	mov eax, 329
	mov rdi[0x520], eax
	mov eax, 330
	mov rdi[0x524], eax
	mov eax, 331
	mov rdi[0x528], eax
	mov eax, 332
	mov rdi[0x52c], eax
	mov eax, 333
	mov rdi[0x530], eax
	mov eax, 334
	mov rdi[0x534], eax
	mov eax, 335
	mov rdi[0x538], eax
	mov eax, 336
	mov rdi[0x53c], eax
	mov eax, 337
	mov rdi[0x540], eax
	mov eax, 338
	mov rdi[0x544], eax
	mov eax, 339
	mov rdi[0x548], eax
	mov eax, 340
	mov rdi[0x54c], eax
	mov eax, 341
	mov rdi[0x550], eax
	mov eax, 342
	mov rdi[0x554], eax
	mov eax, 343
	mov rdi[0x558], eax
	mov eax, 344
	mov rdi[0x55c], eax
	mov eax, 345
	mov rdi[0x560], eax
	mov eax, 346
	mov rdi[0x564], eax
	mov eax, 347
	mov rdi[0x568], eax
	mov eax, 348
	mov rdi[0x56c], eax
	mov eax, 349
	mov rdi[0x570], eax
	mov eax, 350
	mov rdi[0x574], eax
	mov eax, 351
	mov rdi[0x578], eax
	mov eax, 352
	mov rdi[0x57c], eax
	mov eax, 353
	mov rdi[0x580], eax
	mov eax, 354
	mov rdi[0x584], eax
	mov eax, 355
	mov rdi[0x588], eax
	mov eax, 356
	mov rdi[0x58c], eax
	mov eax, 357
	mov rdi[0x590], eax
	mov eax, 358
	mov rdi[0x594], eax
	mov eax, 359
	mov rdi[0x598], eax
	mov eax, 360
	mov rdi[0x59c], eax
	mov eax, 361
	mov rdi[0x5a0], eax
	mov eax, 362
	mov rdi[0x5a4], eax
	mov eax, 363
	mov rdi[0x5a8], eax
	mov eax, 364
	mov rdi[0x5ac], eax
	mov eax, 365
	mov rdi[0x5b0], eax
	mov eax, 366
	mov rdi[0x5b4], eax
	mov eax, 367
	mov rdi[0x5b8], eax
	mov eax, 368
	mov rdi[0x5bc], eax
	mov eax, 369
	mov rdi[0x5c0], eax
	mov eax, 370
	mov rdi[0x5c4], eax
	mov eax, 371
	mov rdi[0x5c8], eax
	mov eax, 372
	mov rdi[0x5cc], eax
	mov eax, 373
	mov rdi[0x5d0], eax
	mov eax, 374
	mov rdi[0x5d4], eax
	mov eax, 375
	mov rdi[0x5d8], eax
	mov eax, 376
	mov rdi[0x5dc], eax
	mov eax, 377
	mov rdi[0x5e0], eax
	mov eax, 378
	mov rdi[0x5e4], eax
	mov eax, 379
	mov rdi[0x5e8], eax
	mov eax, 380
	mov rdi[0x5ec], eax
	mov eax, 381
	mov rdi[0x5f0], eax
	mov eax, 382
	mov rdi[0x5f4], eax
	mov eax, 383
	mov rdi[0x5f8], eax
	mov eax, 384
	mov rdi[0x5fc], eax
	mov eax, 385
	mov rdi[0x600], eax
	mov eax, 386
	mov rdi[0x604], eax
	mov eax, 387
	mov rdi[0x608], eax
	mov eax, 388
	mov rdi[0x60c], eax
	mov eax, 389
	mov rdi[0x610], eax
	mov eax, 390
	mov rdi[0x614], eax
	mov eax, 391
	mov rdi[0x618], eax
	mov eax, 392
	mov rdi[0x61c], eax
	mov eax, 393
	mov rdi[0x620], eax
	mov eax, 394
	mov rdi[0x624], eax
	mov eax, 395
	mov rdi[0x628], eax
	mov eax, 396
	mov rdi[0x62c], eax
	mov eax, 397
	mov rdi[0x630], eax
	mov eax, 398
	mov rdi[0x634], eax
	mov eax, 399
	mov rdi[0x638], eax
	mov eax, 400
	mov rdi[0x63c], eax
	mov eax, 401
	mov rdi[0x640], eax
	mov eax, 402
	mov rdi[0x644], eax
	mov eax, 403
	mov rdi[0x648], eax
	mov eax, 404
	mov rdi[0x64c], eax
	mov eax, 405
	mov rdi[0x650], eax
	mov eax, 406
	mov rdi[0x654], eax
	mov eax, 407
	mov rdi[0x658], eax
	mov eax, 408
	mov rdi[0x65c], eax
	mov eax, 409
	mov rdi[0x660], eax
	mov eax, 410
	mov rdi[0x664], eax
	mov eax, 411
	mov rdi[0x668], eax
	mov eax, 412
	mov rdi[0x66c], eax
	mov eax, 413
	mov rdi[0x670], eax
	mov eax, 414
	mov rdi[0x674], eax
	mov eax, 415
	mov rdi[0x678], eax
	mov eax, 416
	mov rdi[0x67c], eax
	mov eax, 417
	mov rdi[0x680], eax
	mov eax, 418
	mov rdi[0x684], eax
	mov eax, 419
	mov rdi[0x688], eax
	mov eax, 420
	mov rdi[0x68c], eax
	mov eax, 421
	mov rdi[0x690], eax
	mov eax, 422
	mov rdi[0x694], eax
	mov eax, 423
	mov rdi[0x698], eax
	mov eax, 424
	mov rdi[0x69c], eax
	mov eax, 425
	mov rdi[0x6a0], eax
	mov eax, 426
	mov rdi[0x6a4], eax
	mov eax, 427
	mov rdi[0x6a8], eax
	mov eax, 428
	mov rdi[0x6ac], eax
	mov eax, 429
	mov rdi[0x6b0], eax
	mov eax, 430
	mov rdi[0x6b4], eax
	mov eax, 431
	mov rdi[0x6b8], eax
	mov eax, 432
	mov rdi[0x6bc], eax
	mov eax, 433
	mov rdi[0x6c0], eax
	mov eax, 434
	mov rdi[0x6c4], eax
	mov eax, 435
	mov rdi[0x6c8], eax
	mov eax, 436
	mov rdi[0x6cc], eax
	mov eax, 437
	mov rdi[0x6d0], eax
	mov eax, 438
	mov rdi[0x6d4], eax
	mov eax, 439
	mov rdi[0x6d8], eax
	mov eax, 440
	mov rdi[0x6dc], eax
	mov eax, 441
	mov rdi[0x6e0], eax
	mov eax, 442
	mov rdi[0x6e4], eax
	mov eax, 443
	mov rdi[0x6e8], eax
	mov eax, 444
	mov rdi[0x6ec], eax
	mov eax, 445
	mov rdi[0x6f0], eax
	mov eax, 446
	mov rdi[0x6f4], eax
	mov eax, 447
	mov rdi[0x6f8], eax
	mov eax, 448
	mov rdi[0x6fc], eax
	mov eax, 449
	mov rdi[0x700], eax
	mov eax, 450
	mov rdi[0x704], eax
	mov eax, 451
	mov rdi[0x708], eax
	mov eax, 452
	mov rdi[0x70c], eax
	mov eax, 453
	mov rdi[0x710], eax
	mov eax, 454
	mov rdi[0x714], eax
	mov eax, 455
	mov rdi[0x718], eax
	mov eax, 456
	mov rdi[0x71c], eax
	mov eax, 457
	mov rdi[0x720], eax
	mov eax, 458
	mov rdi[0x724], eax
	mov eax, 459
	mov rdi[0x728], eax
	mov eax, 460
	mov rdi[0x72c], eax
	mov eax, 461
	mov rdi[0x730], eax
	mov eax, 462
	mov rdi[0x734], eax
	mov eax, 463
	mov rdi[0x738], eax
	mov eax, 464
	mov rdi[0x73c], eax
	mov eax, 465
	mov rdi[0x740], eax
	mov eax, 466
	mov rdi[0x744], eax
	mov eax, 467
	mov rdi[0x748], eax
	mov eax, 468
	mov rdi[0x74c], eax
	mov eax, 469
	mov rdi[0x750], eax
	mov eax, 470
	mov rdi[0x754], eax
	mov eax, 471
	mov rdi[0x758], eax
	mov eax, 472
	mov rdi[0x75c], eax
	mov eax, 473
	mov rdi[0x760], eax
	mov eax, 474
	mov rdi[0x764], eax
	mov eax, 475
	mov rdi[0x768], eax
	mov eax, 476
	mov rdi[0x76c], eax
	mov eax, 477
	mov rdi[0x770], eax
	mov eax, 478
	mov rdi[0x774], eax
	mov eax, 479
	mov rdi[0x778], eax
	mov eax, 480
	mov rdi[0x77c], eax
	mov eax, 481
	mov rdi[0x780], eax
	mov eax, 482
	mov rdi[0x784], eax
	mov eax, 483
	mov rdi[0x788], eax
	mov eax, 484
	mov rdi[0x78c], eax
	mov eax, 485
	mov rdi[0x790], eax
	mov eax, 486
	mov rdi[0x794], eax
	mov eax, 487
	mov rdi[0x798], eax
	mov eax, 488
	mov rdi[0x79c], eax
	mov eax, 489
	mov rdi[0x7a0], eax
	mov eax, 490
	mov rdi[0x7a4], eax
	mov eax, 491
	mov rdi[0x7a8], eax
	mov eax, 492
	mov rdi[0x7ac], eax
	mov eax, 493
	mov rdi[0x7b0], eax
	mov eax, 494
	mov rdi[0x7b4], eax
	mov eax, 495
	mov rdi[0x7b8], eax
	mov eax, 496
	mov rdi[0x7bc], eax
	mov eax, 497
	mov rdi[0x7c0], eax
	mov eax, 498
	mov rdi[0x7c4], eax
	mov eax, 499
	mov rdi[0x7c8], eax
	mov eax, 500
	mov rdi[0x7cc], eax
	mov eax, 501
	mov rdi[0x7d0], eax
	mov eax, 502
	mov rdi[0x7d4], eax
	mov eax, 503
	mov rdi[0x7d8], eax
	mov eax, 504
	mov rdi[0x7dc], eax
	mov eax, 505
	mov rdi[0x7e0], eax
	mov eax, 506
	mov rdi[0x7e4], eax
	mov eax, 507
	mov rdi[0x7e8], eax
	mov eax, 508
	mov rdi[0x7ec], eax
	mov eax, 509
	mov rdi[0x7f0], eax
	mov eax, 510
	mov rdi[0x7f4], eax
	mov eax, 511
	mov rdi[0x7f8], eax
	mov eax, 512
	mov rdi[0x7fc], eax
	mov eax, 513
	mov rdi[0x800], eax
	mov eax, 514
	mov rdi[0x804], eax
	mov eax, 515
	mov rdi[0x808], eax
	mov eax, 516
	mov rdi[0x80c], eax
	mov eax, 517
	mov rdi[0x810], eax
	mov eax, 518
	mov rdi[0x814], eax
	mov eax, 519
	mov rdi[0x818], eax
	mov eax, 520
	mov rdi[0x81c], eax
	mov eax, 521
	mov rdi[0x820], eax
	mov eax, 522
	mov rdi[0x824], eax
	mov eax, 523
	mov rdi[0x828], eax
	mov eax, 524
	mov rdi[0x82c], eax
	mov eax, 525
	mov rdi[0x830], eax
	mov eax, 526
	mov rdi[0x834], eax
	mov eax, 527
	mov rdi[0x838], eax
	mov eax, 528
	mov rdi[0x83c], eax
	mov eax, 529
	mov rdi[0x840], eax
	mov eax, 530
	mov rdi[0x844], eax
	mov eax, 531
	mov rdi[0x848], eax
	mov eax, 532
	mov rdi[0x84c], eax
	mov eax, 533
	mov rdi[0x850], eax
	mov eax, 534
	mov rdi[0x854], eax
	mov eax, 535
	mov rdi[0x858], eax
	mov eax, 536
	mov rdi[0x85c], eax
	mov eax, 537
	mov rdi[0x860], eax
	mov eax, 538
	mov rdi[0x864], eax
	mov eax, 539
	mov rdi[0x868], eax
	mov eax, 540
	mov rdi[0x86c], eax
	mov eax, 541
	mov rdi[0x870], eax
	mov eax, 542
	mov rdi[0x874], eax
	mov eax, 543
	mov rdi[0x878], eax
	mov eax, 544
	mov rdi[0x87c], eax
	mov eax, 545
	mov rdi[0x880], eax
	mov eax, 546
	mov rdi[0x884], eax
	mov eax, 547
	mov rdi[0x888], eax
	mov eax, 548
	mov rdi[0x88c], eax
	mov eax, 549
	mov rdi[0x890], eax
	mov eax, 550
	mov rdi[0x894], eax
	mov eax, 551
	mov rdi[0x898], eax
	mov eax, 552
	mov rdi[0x89c], eax
	mov eax, 553
	mov rdi[0x8a0], eax
	mov eax, 554
	mov rdi[0x8a4], eax
	mov eax, 555
	mov rdi[0x8a8], eax
	mov eax, 556
	mov rdi[0x8ac], eax
	mov eax, 557
	mov rdi[0x8b0], eax
	mov eax, 558
	mov rdi[0x8b4], eax
	mov eax, 559
	mov rdi[0x8b8], eax
	mov eax, 560
	mov rdi[0x8bc], eax
	mov eax, 561
	mov rdi[0x8c0], eax
	mov eax, 562
	mov rdi[0x8c4], eax
	mov eax, 563
	mov rdi[0x8c8], eax
	mov eax, 564
	mov rdi[0x8cc], eax
	mov eax, 565
	mov rdi[0x8d0], eax
	mov eax, 566
	mov rdi[0x8d4], eax
	mov eax, 567
	mov rdi[0x8d8], eax
	mov eax, 568
	mov rdi[0x8dc], eax
	mov eax, 569
	mov rdi[0x8e0], eax
	mov eax, 570
	mov rdi[0x8e4], eax
	mov eax, 571
	mov rdi[0x8e8], eax
	mov eax, 572
	mov rdi[0x8ec], eax
	mov eax, 573
	mov rdi[0x8f0], eax
	mov eax, 574
	mov rdi[0x8f4], eax
	mov eax, 575
	mov rdi[0x8f8], eax
	mov eax, 576
	mov rdi[0x8fc], eax
	mov eax, 577
	mov rdi[0x900], eax
	mov eax, 578
	mov rdi[0x904], eax
	mov eax, 579
	mov rdi[0x908], eax
	mov eax, 580
	mov rdi[0x90c], eax
	mov eax, 581
	mov rdi[0x910], eax
	mov eax, 582
	mov rdi[0x914], eax
	mov eax, 583
	mov rdi[0x918], eax
	mov eax, 584
	mov rdi[0x91c], eax
	mov eax, 585
	mov rdi[0x920], eax
	mov eax, 586
	mov rdi[0x924], eax
	mov eax, 587
	mov rdi[0x928], eax
	mov eax, 588
	mov rdi[0x92c], eax
	mov eax, 589
	mov rdi[0x930], eax
	mov eax, 590
	mov rdi[0x934], eax
	mov eax, 591
	mov rdi[0x938], eax
	mov eax, 592
	mov rdi[0x93c], eax
	mov eax, 593
	mov rdi[0x940], eax
	mov eax, 594
	mov rdi[0x944], eax
	mov eax, 595
	mov rdi[0x948], eax
	mov eax, 596
	mov rdi[0x94c], eax
	mov eax, 597
	mov rdi[0x950], eax
	mov eax, 598
	mov rdi[0x954], eax
	mov eax, 599
	mov rdi[0x958], eax
	mov eax, 600
	mov rdi[0x95c], eax
	mov eax, 601
	mov rdi[0x960], eax
	mov eax, 602
	mov rdi[0x964], eax
	mov eax, 603
	mov rdi[0x968], eax
	mov eax, 604
	mov rdi[0x96c], eax
	mov eax, 605
	mov rdi[0x970], eax
	mov eax, 606
	mov rdi[0x974], eax
	mov eax, 607
	mov rdi[0x978], eax
	mov eax, 608
	mov rdi[0x97c], eax
	mov eax, 609
	mov rdi[0x980], eax
	mov eax, 610
	mov rdi[0x984], eax
	mov eax, 611
	mov rdi[0x988], eax
	mov eax, 612
	mov rdi[0x98c], eax
	mov eax, 613
	mov rdi[0x990], eax
	mov eax, 614
	mov rdi[0x994], eax
	mov eax, 615
	mov rdi[0x998], eax
	mov eax, 616
	mov rdi[0x99c], eax
	mov eax, 617
	mov rdi[0x9a0], eax
	mov eax, 618
	mov rdi[0x9a4], eax
	mov eax, 619
	mov rdi[0x9a8], eax
	mov eax, 620
	mov rdi[0x9ac], eax
	mov eax, 621
	mov rdi[0x9b0], eax
	mov eax, 622
	mov rdi[0x9b4], eax
	mov eax, 623
	mov rdi[0x9b8], eax
	mov eax, 624
	mov rdi[0x9bc], eax
	mov eax, 625
	mov rdi[0x9c0], eax
	mov eax, 626
	mov rdi[0x9c4], eax
	mov eax, 627
	mov rdi[0x9c8], eax
	mov eax, 628
	mov rdi[0x9cc], eax
	mov eax, 629
	mov rdi[0x9d0], eax
	mov eax, 630
	mov rdi[0x9d4], eax
	mov eax, 631
	mov rdi[0x9d8], eax
	mov eax, 632
	mov rdi[0x9dc], eax
	mov eax, 633
	mov rdi[0x9e0], eax
	mov eax, 634
	mov rdi[0x9e4], eax
	mov eax, 635
	mov rdi[0x9e8], eax
	mov eax, 636
	mov rdi[0x9ec], eax
	mov eax, 637
	mov rdi[0x9f0], eax
	mov eax, 638
	mov rdi[0x9f4], eax
	mov eax, 639
	mov rdi[0x9f8], eax
	mov eax, 640
	mov rdi[0x9fc], eax
	mov eax, 641
	mov rdi[0xa00], eax
	mov eax, 642
	mov rdi[0xa04], eax
	mov eax, 643
	mov rdi[0xa08], eax
	mov eax, 644
	mov rdi[0xa0c], eax
	mov eax, 645
	mov rdi[0xa10], eax
	mov eax, 646
	mov rdi[0xa14], eax
	mov eax, 647
	mov rdi[0xa18], eax
	mov eax, 648
	mov rdi[0xa1c], eax
	mov eax, 649
	mov rdi[0xa20], eax
	mov eax, 650
	mov rdi[0xa24], eax
	mov eax, 651
	mov rdi[0xa28], eax
	mov eax, 652
	mov rdi[0xa2c], eax
	mov eax, 653
	mov rdi[0xa30], eax
	mov eax, 654
	mov rdi[0xa34], eax
	mov eax, 655
	mov rdi[0xa38], eax
	mov eax, 656
	mov rdi[0xa3c], eax
	mov eax, 657
	mov rdi[0xa40], eax
	mov eax, 658
	mov rdi[0xa44], eax
	mov eax, 659
	mov rdi[0xa48], eax
	mov eax, 660
	mov rdi[0xa4c], eax
	mov eax, 661
	mov rdi[0xa50], eax
	mov eax, 662
	mov rdi[0xa54], eax
	mov eax, 663
	mov rdi[0xa58], eax
	mov eax, 664
	mov rdi[0xa5c], eax
	mov eax, 665
	mov rdi[0xa60], eax
	mov eax, 666
	mov rdi[0xa64], eax
	mov eax, 667
	mov rdi[0xa68], eax
	mov eax, 668
	mov rdi[0xa6c], eax
	mov eax, 669
	mov rdi[0xa70], eax
	mov eax, 670
	mov rdi[0xa74], eax
	mov eax, 671
	mov rdi[0xa78], eax
	mov eax, 672
	mov rdi[0xa7c], eax
	mov eax, 673
	mov rdi[0xa80], eax
	mov eax, 674
	mov rdi[0xa84], eax
	mov eax, 675
	mov rdi[0xa88], eax
	mov eax, 676
	mov rdi[0xa8c], eax
	mov eax, 677
	mov rdi[0xa90], eax
	mov eax, 678
	mov rdi[0xa94], eax
	mov eax, 679
	mov rdi[0xa98], eax
	mov eax, 680
	mov rdi[0xa9c], eax
	mov eax, 681
	mov rdi[0xaa0], eax
	mov eax, 682
	mov rdi[0xaa4], eax
	mov eax, 683
	mov rdi[0xaa8], eax
	mov eax, 684
	mov rdi[0xaac], eax
	mov eax, 685
	mov rdi[0xab0], eax
	mov eax, 686
	mov rdi[0xab4], eax
	mov eax, 687
	mov rdi[0xab8], eax
	mov eax, 688
	mov rdi[0xabc], eax
	mov eax, 689
	mov rdi[0xac0], eax
	mov eax, 690
	mov rdi[0xac4], eax
	mov eax, 691
	mov rdi[0xac8], eax
	mov eax, 692
	mov rdi[0xacc], eax
	mov eax, 693
	mov rdi[0xad0], eax
	mov eax, 694
	mov rdi[0xad4], eax
	mov eax, 695
	mov rdi[0xad8], eax
	mov eax, 696
	mov rdi[0xadc], eax
	mov eax, 697
	mov rdi[0xae0], eax
	mov eax, 698
	mov rdi[0xae4], eax
	mov eax, 699
	mov rdi[0xae8], eax
	mov eax, 700
	mov rdi[0xaec], eax
	mov eax, 701
	mov rdi[0xaf0], eax
	mov eax, 702
	mov rdi[0xaf4], eax
	mov eax, 703
	mov rdi[0xaf8], eax
	mov eax, 704
	mov rdi[0xafc], eax
	mov eax, 705
	mov rdi[0xb00], eax
	mov eax, 706
	mov rdi[0xb04], eax
	mov eax, 707
	mov rdi[0xb08], eax
	mov eax, 708
	mov rdi[0xb0c], eax
	mov eax, 709
	mov rdi[0xb10], eax
	mov eax, 710
	mov rdi[0xb14], eax
	mov eax, 711
	mov rdi[0xb18], eax
	mov eax, 712
	mov rdi[0xb1c], eax
	mov eax, 713
	mov rdi[0xb20], eax
	mov eax, 714
	mov rdi[0xb24], eax
	mov eax, 715
	mov rdi[0xb28], eax
	mov eax, 716
	mov rdi[0xb2c], eax
	mov eax, 717
	mov rdi[0xb30], eax
	mov eax, 718
	mov rdi[0xb34], eax
	mov eax, 719
	mov rdi[0xb38], eax
	mov eax, 720
	mov rdi[0xb3c], eax
	mov eax, 721
	mov rdi[0xb40], eax
	mov eax, 722
	mov rdi[0xb44], eax
	mov eax, 723
	mov rdi[0xb48], eax
	mov eax, 724
	mov rdi[0xb4c], eax
	mov eax, 725
	mov rdi[0xb50], eax
	mov eax, 726
	mov rdi[0xb54], eax
	mov eax, 727
	mov rdi[0xb58], eax
	mov eax, 728
	mov rdi[0xb5c], eax
	mov eax, 729
	mov rdi[0xb60], eax
	mov eax, 730
	mov rdi[0xb64], eax
	mov eax, 731
	mov rdi[0xb68], eax
	mov eax, 732
	mov rdi[0xb6c], eax
	mov eax, 733
	mov rdi[0xb70], eax
	mov eax, 734
	mov rdi[0xb74], eax
	mov eax, 735
	mov rdi[0xb78], eax
	mov eax, 736
	mov rdi[0xb7c], eax
	mov eax, 737
	mov rdi[0xb80], eax
	mov eax, 738
	mov rdi[0xb84], eax
	mov eax, 739
	mov rdi[0xb88], eax
	mov eax, 740
	mov rdi[0xb8c], eax
	mov eax, 741
	mov rdi[0xb90], eax
	mov eax, 742
	mov rdi[0xb94], eax
	mov eax, 743
	mov rdi[0xb98], eax
	mov eax, 744
	mov rdi[0xb9c], eax
	mov eax, 745
	mov rdi[0xba0], eax
	mov eax, 746
	mov rdi[0xba4], eax
	mov eax, 747
	mov rdi[0xba8], eax
	mov eax, 748
	mov rdi[0xbac], eax
	mov eax, 749
	mov rdi[0xbb0], eax
	mov eax, 750
	mov rdi[0xbb4], eax
	mov eax, 751
	mov rdi[0xbb8], eax
	mov eax, 752
	mov rdi[0xbbc], eax
	mov eax, 753
	mov rdi[0xbc0], eax
	mov eax, 754
	mov rdi[0xbc4], eax
	mov eax, 755
	mov rdi[0xbc8], eax
	mov eax, 756
	mov rdi[0xbcc], eax
	mov eax, 757
	mov rdi[0xbd0], eax
	mov eax, 758
	mov rdi[0xbd4], eax
	mov eax, 759
	mov rdi[0xbd8], eax
	mov eax, 760
	mov rdi[0xbdc], eax
	mov eax, 761
	mov rdi[0xbe0], eax
	mov eax, 762
	mov rdi[0xbe4], eax
	mov eax, 763
	mov rdi[0xbe8], eax
	mov eax, 764
	mov rdi[0xbec], eax
	mov eax, 765
	mov rdi[0xbf0], eax
	mov eax, 766
	mov rdi[0xbf4], eax
	mov eax, 767
	mov rdi[0xbf8], eax
	mov eax, 768
	mov rdi[0xbfc], eax
	mov eax, 769
	mov rdi[0xc00], eax
	mov eax, 770
	mov rdi[0xc04], eax
	mov eax, 771
	mov rdi[0xc08], eax
	mov eax, 772
	mov rdi[0xc0c], eax
	mov eax, 773
	mov rdi[0xc10], eax
	mov eax, 774
	mov rdi[0xc14], eax
	mov eax, 775
	mov rdi[0xc18], eax
	mov eax, 776
	mov rdi[0xc1c], eax
	mov eax, 777
	mov rdi[0xc20], eax
	mov eax, 778
	mov rdi[0xc24], eax
	mov eax, 779
	mov rdi[0xc28], eax
	mov eax, 780
	mov rdi[0xc2c], eax
	mov eax, 781
	mov rdi[0xc30], eax
	mov eax, 782
	mov rdi[0xc34], eax
	mov eax, 783
	mov rdi[0xc38], eax
	mov eax, 784
	mov rdi[0xc3c], eax
	mov eax, 785
	mov rdi[0xc40], eax
	mov eax, 786
	mov rdi[0xc44], eax
	mov eax, 787
	mov rdi[0xc48], eax
	mov eax, 788
	mov rdi[0xc4c], eax
	mov eax, 789
	mov rdi[0xc50], eax
	mov eax, 790
	mov rdi[0xc54], eax
	mov eax, 791
	mov rdi[0xc58], eax
	mov eax, 792
	mov rdi[0xc5c], eax
	mov eax, 793
	mov rdi[0xc60], eax
	mov eax, 794
	mov rdi[0xc64], eax
	mov eax, 795
	mov rdi[0xc68], eax
	mov eax, 796
	mov rdi[0xc6c], eax
	mov eax, 797
	mov rdi[0xc70], eax
	mov eax, 798
	mov rdi[0xc74], eax
	mov eax, 799
	mov rdi[0xc78], eax
	mov eax, 800
	mov rdi[0xc7c], eax
	mov eax, 801
	mov rdi[0xc80], eax
	mov eax, 802
	mov rdi[0xc84], eax
	mov eax, 803
	mov rdi[0xc88], eax
	mov eax, 804
	mov rdi[0xc8c], eax
	mov eax, 805
	mov rdi[0xc90], eax
	mov eax, 806
	mov rdi[0xc94], eax
	mov eax, 807
	mov rdi[0xc98], eax
	mov eax, 808
	mov rdi[0xc9c], eax
	mov eax, 809
	mov rdi[0xca0], eax
	mov eax, 810
	mov rdi[0xca4], eax
	mov eax, 811
	mov rdi[0xca8], eax
	mov eax, 812
	mov rdi[0xcac], eax
	mov eax, 813
	mov rdi[0xcb0], eax
	mov eax, 814
	mov rdi[0xcb4], eax
	mov eax, 815
	mov rdi[0xcb8], eax
	mov eax, 816
	mov rdi[0xcbc], eax
	mov eax, 817
	mov rdi[0xcc0], eax
	mov eax, 818
	mov rdi[0xcc4], eax
	mov eax, 819
	mov rdi[0xcc8], eax
	mov eax, 820
	mov rdi[0xccc], eax
	mov eax, 821
	mov rdi[0xcd0], eax
	mov eax, 822
	mov rdi[0xcd4], eax
	mov eax, 823
	mov rdi[0xcd8], eax
	mov eax, 824
	mov rdi[0xcdc], eax
	mov eax, 825
	mov rdi[0xce0], eax
	mov eax, 826
	mov rdi[0xce4], eax
	mov eax, 827
	mov rdi[0xce8], eax
	mov eax, 828
	mov rdi[0xcec], eax
	mov eax, 829
	mov rdi[0xcf0], eax
	mov eax, 830
	mov rdi[0xcf4], eax
	mov eax, 831
	mov rdi[0xcf8], eax
	mov eax, 832
	mov rdi[0xcfc], eax
	mov eax, 833
	mov rdi[0xd00], eax
	mov eax, 834
	mov rdi[0xd04], eax
	mov eax, 835
	mov rdi[0xd08], eax
	mov eax, 836
	mov rdi[0xd0c], eax
	mov eax, 837
	mov rdi[0xd10], eax
	mov eax, 838
	mov rdi[0xd14], eax
	mov eax, 839
	mov rdi[0xd18], eax
	mov eax, 840
	mov rdi[0xd1c], eax
	mov eax, 841
	mov rdi[0xd20], eax
	mov eax, 842
	mov rdi[0xd24], eax
	mov eax, 843
	mov rdi[0xd28], eax
	mov eax, 844
	mov rdi[0xd2c], eax
	mov eax, 845
	mov rdi[0xd30], eax
	mov eax, 846
	mov rdi[0xd34], eax
	mov eax, 847
	mov rdi[0xd38], eax
	mov eax, 848
	mov rdi[0xd3c], eax
	mov eax, 849
	mov rdi[0xd40], eax
	mov eax, 850
	mov rdi[0xd44], eax
	mov eax, 851
	mov rdi[0xd48], eax
	mov eax, 852
	mov rdi[0xd4c], eax
	mov eax, 853
	mov rdi[0xd50], eax
	mov eax, 854
	mov rdi[0xd54], eax
	mov eax, 855
	mov rdi[0xd58], eax
	mov eax, 856
	mov rdi[0xd5c], eax
	mov eax, 857
	mov rdi[0xd60], eax
	mov eax, 858
	mov rdi[0xd64], eax
	mov eax, 859
	mov rdi[0xd68], eax
	mov eax, 860
	mov rdi[0xd6c], eax
	mov eax, 861
	mov rdi[0xd70], eax
	mov eax, 862
	mov rdi[0xd74], eax
	mov eax, 863
	mov rdi[0xd78], eax
	mov eax, 864
	mov rdi[0xd7c], eax
	mov eax, 865
	mov rdi[0xd80], eax
	mov eax, 866
	mov rdi[0xd84], eax
	mov eax, 867
	mov rdi[0xd88], eax
	mov eax, 868
	mov rdi[0xd8c], eax
	mov eax, 869
	mov rdi[0xd90], eax
	mov eax, 870
	mov rdi[0xd94], eax
	mov eax, 871
	mov rdi[0xd98], eax
	mov eax, 872
	mov rdi[0xd9c], eax
	mov eax, 873
	mov rdi[0xda0], eax
	mov eax, 874
	mov rdi[0xda4], eax
	mov eax, 875
	mov rdi[0xda8], eax
	mov eax, 876
	mov rdi[0xdac], eax
	mov eax, 877
	mov rdi[0xdb0], eax
	mov eax, 878
	mov rdi[0xdb4], eax
	mov eax, 879
	mov rdi[0xdb8], eax
	mov eax, 880
	mov rdi[0xdbc], eax
	mov eax, 881
	mov rdi[0xdc0], eax
	mov eax, 882
	mov rdi[0xdc4], eax
	mov eax, 883
	mov rdi[0xdc8], eax
	mov eax, 884
	mov rdi[0xdcc], eax
	mov eax, 885
	mov rdi[0xdd0], eax
	mov eax, 886
	mov rdi[0xdd4], eax
	mov eax, 887
	mov rdi[0xdd8], eax
	mov eax, 888
	mov rdi[0xddc], eax
	mov eax, 889
	mov rdi[0xde0], eax
	mov eax, 890
	mov rdi[0xde4], eax
	mov eax, 891
	mov rdi[0xde8], eax
	mov eax, 892
	mov rdi[0xdec], eax
	mov eax, 893
	mov rdi[0xdf0], eax
	mov eax, 894
	mov rdi[0xdf4], eax
	mov eax, 895
	mov rdi[0xdf8], eax
	mov eax, 896
	mov rdi[0xdfc], eax
	mov eax, 897
	mov rdi[0xe00], eax
	mov eax, 898
	mov rdi[0xe04], eax
	mov eax, 899
	mov rdi[0xe08], eax
	mov eax, 900
	mov rdi[0xe0c], eax
	mov eax, 901
	mov rdi[0xe10], eax
	mov eax, 902
	mov rdi[0xe14], eax
	mov eax, 903
	mov rdi[0xe18], eax
	mov eax, 904
	mov rdi[0xe1c], eax
	mov eax, 905
	mov rdi[0xe20], eax
	mov eax, 906
	mov rdi[0xe24], eax
	mov eax, 907
	mov rdi[0xe28], eax
	mov eax, 908
	mov rdi[0xe2c], eax
	mov eax, 909
	mov rdi[0xe30], eax
	mov eax, 910
	mov rdi[0xe34], eax
	mov eax, 911
	mov rdi[0xe38], eax
	mov eax, 912
	mov rdi[0xe3c], eax
	mov eax, 913
	mov rdi[0xe40], eax
	mov eax, 914
	mov rdi[0xe44], eax
	mov eax, 915
	mov rdi[0xe48], eax
	mov eax, 916
	mov rdi[0xe4c], eax
	mov eax, 917
	mov rdi[0xe50], eax
	mov eax, 918
	mov rdi[0xe54], eax
	mov eax, 919
	mov rdi[0xe58], eax
	mov eax, 920
	mov rdi[0xe5c], eax
	mov eax, 921
	mov rdi[0xe60], eax
	mov eax, 922
	mov rdi[0xe64], eax
	mov eax, 923
	mov rdi[0xe68], eax
	mov eax, 924
	mov rdi[0xe6c], eax
	mov eax, 925
	mov rdi[0xe70], eax
	mov eax, 926
	mov rdi[0xe74], eax
	mov eax, 927
	mov rdi[0xe78], eax
	mov eax, 928
	mov rdi[0xe7c], eax
	mov eax, 929
	mov rdi[0xe80], eax
	mov eax, 930
	mov rdi[0xe84], eax
	mov eax, 931
	mov rdi[0xe88], eax
	mov eax, 932
	mov rdi[0xe8c], eax
	mov eax, 933
	mov rdi[0xe90], eax
	mov eax, 934
	mov rdi[0xe94], eax
	mov eax, 935
	mov rdi[0xe98], eax
	mov eax, 936
	mov rdi[0xe9c], eax
	mov eax, 937
	mov rdi[0xea0], eax
	mov eax, 938
	mov rdi[0xea4], eax
	mov eax, 939
	mov rdi[0xea8], eax
	mov eax, 940
	mov rdi[0xeac], eax
	mov eax, 941
	mov rdi[0xeb0], eax
	mov eax, 942
	mov rdi[0xeb4], eax
	mov eax, 943
	mov rdi[0xeb8], eax
	mov eax, 944
	mov rdi[0xebc], eax
	mov eax, 945
	mov rdi[0xec0], eax
	mov eax, 946
	mov rdi[0xec4], eax
	mov eax, 947
	mov rdi[0xec8], eax
	mov eax, 948
	mov rdi[0xecc], eax
	mov eax, 949
	mov rdi[0xed0], eax
	mov eax, 950
	mov rdi[0xed4], eax
	mov eax, 951
	mov rdi[0xed8], eax
	mov eax, 952
	mov rdi[0xedc], eax
	mov eax, 953
	mov rdi[0xee0], eax
	mov eax, 954
	mov rdi[0xee4], eax
	mov eax, 955
	mov rdi[0xee8], eax
	mov eax, 956
	mov rdi[0xeec], eax
	mov eax, 957
	mov rdi[0xef0], eax
	mov eax, 958
	mov rdi[0xef4], eax
	mov eax, 959
	mov rdi[0xef8], eax
	mov eax, 960
	mov rdi[0xefc], eax
	mov eax, 961
	mov rdi[0xf00], eax
	mov eax, 962
	mov rdi[0xf04], eax
	mov eax, 963
	mov rdi[0xf08], eax
	mov eax, 964
	mov rdi[0xf0c], eax
	mov eax, 965
	mov rdi[0xf10], eax
	mov eax, 966
	mov rdi[0xf14], eax
	mov eax, 967
	mov rdi[0xf18], eax
	mov eax, 968
	mov rdi[0xf1c], eax
	mov eax, 969
	mov rdi[0xf20], eax
	mov eax, 970
	mov rdi[0xf24], eax
	mov eax, 971
	mov rdi[0xf28], eax
	mov eax, 972
	mov rdi[0xf2c], eax
	mov eax, 973
	mov rdi[0xf30], eax
	mov eax, 974
	mov rdi[0xf34], eax
	mov eax, 975
	mov rdi[0xf38], eax
	mov eax, 976
	mov rdi[0xf3c], eax
	mov eax, 977
	mov rdi[0xf40], eax
	mov eax, 978
	mov rdi[0xf44], eax
	mov eax, 979
	mov rdi[0xf48], eax
	mov eax, 980
	mov rdi[0xf4c], eax
	mov eax, 981
	mov rdi[0xf50], eax
	mov eax, 982
	mov rdi[0xf54], eax
	mov eax, 983
	mov rdi[0xf58], eax
	mov eax, 984
	mov rdi[0xf5c], eax
	mov eax, 985
	mov rdi[0xf60], eax
	mov eax, 986
	mov rdi[0xf64], eax
	mov eax, 987
	mov rdi[0xf68], eax
	mov eax, 988
	mov rdi[0xf6c], eax
	mov eax, 989
	mov rdi[0xf70], eax
	mov eax, 990
	mov rdi[0xf74], eax
	mov eax, 991
	mov rdi[0xf78], eax
	mov eax, 992
	mov rdi[0xf7c], eax
	mov eax, 993
	mov rdi[0xf80], eax
	mov eax, 994
	mov rdi[0xf84], eax
	mov eax, 995
	mov rdi[0xf88], eax
	mov eax, 996
	mov rdi[0xf8c], eax
	mov eax, 997
	mov rdi[0xf90], eax
	mov eax, 998
	mov rdi[0xf94], eax
	mov eax, 999
	mov rdi[0xf98], eax
	mov eax, 1000
	mov rdi[0xf9c], eax
	ret
