# grug tests

This is the test suite of [my programming language called grug](https://github.com/MyNameIsTrez/grug/).

## Running

1. Clone this repository and `cd` into it.
2. Run `git submodule update --init` to clone the `grug.c` and `grug.h` files (for your own game you can just drop these files directly into your project).
3. Run the tests with `./tests.sh`.

Check the terminal or the generated `results` directory in tests to inspect any errors.

If you want to see the detailed logs, run `LOGGING= ./tests.sh`.

If you want to allow your compiler to optimize `grug.c` hard, run `OPTIMIZED= ./tests.sh`.
If you want to allow your compiler to optimize `grug.c` extra hard, run `OPTIMIZED= CRASH_ON_UNREACHABLE= ./tests.sh`.
Use `objdump --section=.text -d optimized.out > optimized.s` with `diff optimized.s optimized_crash_on_unreachable.s > diff.s` if you want to compare the generated sections.

You can do `./tests.sh [test_path]` to run a specific test, like `./tests.sh tests_ok/minimal`

Once you've ran `./tests.sh`, you can run `objdump -D tests_ok/helper_fn/results/expected.so -M intel` to display the assembler contents grug is expected to produce.

Note that if you're using a Debian-based distribution like Ubuntu 22.04, you might need to run `sudo sysctl vm.mmap_rnd_bits=28` to fix address sanitizer, which `tests.sh` uses. See [this GitHub thread](https://github.com/actions/runner-images/issues/9524#issuecomment-2002475952) for context.

## Dependencies

`tests.sh` requires nasm and ld, and has been tested with the below versions, though later versions of nasm and ld should work too:

- NASM version 2.15.05
- GNU ld (GNU Binutils for Ubuntu) 2.38

## Run tests.sh on save

1. Install this [Run on Save](https://marketplace.visualstudio.com/items?itemName=emeraldwalk.RunOnSave) extension for VS Code.
2. In the Output window, which is one of the buttons next to the Terminal button, press the dropdown on the right and select `Run On Save`. Here you will see the output of the tests every time you save.

## Inspecting the tokens and AST

```bash
gcc run.c grug/grug.c -Wall -Wextra -Werror -Wpedantic -Wshadow -Wfatal-errors -g -Igrug -fsanitize=address,undefined -DLOGGING && \
./a.out tests_ok/helper_fn/input.grug tests_ok/helper_fn/results/expected.so
```

## gdb

```bash
gdb --args a.out tests_ok/helper_fn/input.grug tests_ok/helper_fn/results/expected.so
```

## gdb on a test

```bash
nasm tests_ok/write_to_global_variable/input.s -f elf64 -o tests_ok/write_to_global_variable/results/expected.o &&\
ld -shared --hash-style=sysv tests_ok/write_to_global_variable/results/expected.o -o tests_ok/write_to_global_variable/results/expected.so &&\
rm tests_ok/write_to_global_variable/results/expected.o &&\
gcc tests_ok/write_to_global_variable/test.c -Igrug -std=gnu2x -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wuninitialized -Wfatal-errors -g -Og -rdynamic -o tests_ok/write_to_global_variable/results/test &&\
gdb --args tests_ok/write_to_global_variable/results/test tests_ok/write_to_global_variable/results/expected.so
```

## gdbgui

Use [gdbgui](https://www.gdbgui.com/) to step through the code:

```bash
gdbgui "a.out tests_ok/helper_fn/input.grug tests_ok/helper_fn/results/expected.so"
```

## readelf

```bash
clear && ./tests.sh tests_ok/helper_fn
```

## Manually doing call address calculations

Getting the address of a CALL instruction:

```bash
echo 'obase=16;ibase=16;FFFFFFFB-(call_insn_line-plt_fn_line)' | bc
```

so:

```bash
echo 'obase=16;ibase=16;FFFFFFFB-(1055-1020)' | bc
```
