# grug tests

`grug.c` is split up into sections, which you can find by searching for `////`:

1. JSON
2. INIT
3. READING
4. TOKENIZATION
5. PARSING
6. SERIALIZING TO C
7. COMPILING
8. MACHINING
9. LINKING
10. MISC

# Running

1. Clone this repository and enter it.
2. Run `git submodule update --init` to clone the `grug.c` and `grug.h` files (for your own game you can just drop these files directly into your project).
3. Run the tests with `./tests.sh`.

Check the terminal or the generated `results` directory to inspect any errors.

If you want to see the detailed logs, add `-DLOGGING` at the end of line 5 of `tests.sh`.
