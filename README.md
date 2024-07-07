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

Note that if you're using a Debian-based distribution like Ubuntu 22.04, you might need to run `sudo sysctl vm.mmap_rnd_bits=28` to fix address sanitizer, which `tests.sh` uses. See [this GitHub thread](https://github.com/actions/runner-images/issues/9524#issuecomment-2002475952) for context.

## gdb

`gdb --args a.out tests_ok/globals/input.grug results/expected.so`

## gdbgui

Use [gdbgui](https://www.gdbgui.com/) to step through the code:

`gdbgui "a.out tests_ok/globals/input.grug results/expected.so"`

## readelf

`clear && ./tests.sh tests_ok/globals; readelf -a results/output.so > output_elf.hex && readelf -a results/expected.so > expected_elf.hex`
