# grug tests

This is the test suite of [my programming language called grug](https://github.com/MyNameIsTrez/grug/).

## Running

1. Clone this repository and enter it.
2. Run `git submodule update --init` to clone the `grug.c` and `grug.h` files (for your own game you can just drop these files directly into your project).
3. Run the tests with `./tests.sh`.

Check the terminal or the generated `results` directory to inspect any errors.

If you want to see the detailed logs, add `-DLOGGING` at the end of line 5 of `tests.sh`.

You can do `./tests.sh [test_path]` to run a specific test, like `./tests.sh tests_ok/minimal`

Once you've ran `./tests.sh`, you can run `objdump -D results/expected.so` to display the assembler contents grug is expected to produce.

## gdb

`gdb --args a.out tests_ok/define_containing_string/input.grug results/expected.so`

## gdbgui

Use [gdbgui](https://www.gdbgui.com/) to step through the code:

`gdbgui "a.out tests_ok/on_fn_entity_b_on_b/input.grug results/expected.so"`

## readelf

`clear && ./tests.sh tests_ok/define_containing_string; readelf -a results/output.so > output_elf.hex && readelf -a results/expected.so > expected_elf.hex`
