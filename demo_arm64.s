// Assemble: clang demo_arm64.s --target=arm64 -shared
// Assemble: as demo_arm64.s
// Assemble: llvm-mc-14 --arch=arm64 --filetype=obj -o demo_arm64.o demo_arm64.s

.data

.global define_type
define_type:
    .ascii "a"

.balign 8
.global globals_size
globals_size:
    .xword 0

// .global on_fns
// on_fns:
//     .xword _magic

.global strings
strings:

.balign 8
.global resources_size
resources_size:
    .xword 0

.text

// TODO: USE
.extern grug_fn_name
// TODO: USE
.extern grug_fn_path
// TODO: USE
.extern grug_block_mask

// TODO: USE
// .extern game_fn_define_a[WEAK]

.global define
define:
	// bl game_fn_define_a
	ret

.balign 16
.global init_globals
init_globals:
    ret

// .balign 16
// .global magic
// magic:
//     mov w0, 42
//     ret
