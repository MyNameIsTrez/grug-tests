// Assemble: clang magic.s --target=arm64 -shared

.data

.global _define_type
_define_type:
    .ascii "a"

.balign 8
.global _globals_size
_globals_size:
    .xword 0

// .global _on_fns
// _on_fns:
//     .xword _magic

.global _strings
_strings:

.balign 8
.global _resources_size
_resources_size:
    .xword 0

.text

// TODO: USE
.extern _grug_on_fn_name
// TODO: USE
.extern _grug_on_fn_path
// TODO: USE
.extern _grug_block_mask

.extern game_fn_define_a

.global _define
_define:
	bl game_fn_define_a
	ret

.balign 16
.global _init_globals
_init_globals:
    ret

// .balign 16
// .global _magic
// _magic:
//     mov w0, 42
//     ret
