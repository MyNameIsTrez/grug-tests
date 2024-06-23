# grug tests

This is the test suite of [my programming language called grug](https://github.com/MyNameIsTrez/grug/).

## Running

1. Clone this repository and enter it.
2. Run `git submodule update --init` to clone the `grug.c` and `grug.h` files (for your own game you can just drop these files directly into your project).
3. Run the tests with `./tests.sh`.

Check the terminal or the generated `results` directory to inspect any errors.

If you want to see the detailed logs, add `-DLOGGING` at the end of line 5 of `tests.sh`.

Once you've ran `./tests.sh`, you can run `objdump -D results/expected.so` to display the assembler contents grug is expected to produce.
