# Benchmark these before sticking with them

- If the jump's destination is less than 256 bytes away, use the shorter `je` instruction
- If an i64 number is below the i32 max, use `mov eax, n`, instead of `mov rax, n` (where `xor eax, eax` is still used if `n == 0`)
- Can `push rax` sometimes be replaced with `push eax`?
- Can `pop rdi` sometimes be replaced with `pop edi`?
- Can `add rax, rbx` sometimes be replaced with `add eax, ebx`?

# Implemented optimizations

- If the number/bool is 0, use `xor eax, eax` (`XOR_CLEAR_EAX = 0xc031,`), rather than the current `mov rax, 0` (`MOVABS_TO_RAX = 0x0xb848,` plus `compile_push_number(expr.literal.i32, 8);`)
- If the number/bool is 1, use `mov eax, 1` (`MOV_1_TO_EAX = 0x1b8,`), rather than the current `mov rax, 1` (`MOVABS_TO_RAX = 0x0xb848,` plus `compile_push_number(expr.literal.i32, 8);`)
