# grug tests

This is the test suite of [my programming language called grug](https://github.com/MyNameIsTrez/grug/).

## Running

1. Clone this repository and enter it.
2. Run `git submodule update --init` to clone the `grug.c` and `grug.h` files (for your own game you can just drop these files directly into your project).
3. Run the tests with `./tests.sh`.

Check the terminal or the generated `results` directory to inspect any errors.

If you want to see the detailed logs, add `-DLOGGING` at the end of line 5 of `tests.sh`.

Once you've ran `./tests.sh`, you can run `objdump -D results/expected.so` to display the assembler contents grug is expected to produce.

## gdbgui

Use [gdbgui](https://www.gdbgui.com/) to step through the code:

`gdbgui "a.out tests_err/define_fn_different_name/input.grug results/expected.so"`
`gdbgui "a.out tests_ok/on_fn/input.grug results/expected.so"`

## readelf

`clear && ./tests.sh; readelf -a results/output.so > output_elf.hex && readelf -a results/expected.so > expected_elf.hex`
