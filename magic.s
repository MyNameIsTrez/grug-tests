// Assemble: clang magic.s --target=arm64 -shared

.data

.global _on_fns
on_fns:
    .xword _magic

.text

.global _magic
.balign 16
_magic:
    mov w0, 42
    ret
