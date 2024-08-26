# Development

## Notes

If you want to see the detailed logs, run `LOGGING= ./tests.sh`.

If you want to allow your compiler to optimize `grug.c` hard, run `OPTIMIZED= ./tests.sh`.
If you want to allow your compiler to optimize `grug.c` extra hard, run `OPTIMIZED= CRASH_ON_UNREACHABLE= ./tests.sh`.
Use `objdump --section=.text -d optimized.out > optimized.s` with `diff optimized.s optimized_crash_on_unreachable.s > diff.s` if you want to compare the generated sections.

You can do `./tests.sh [test_path]` to run a specific test, like `./tests.sh tests_ok/minimal`

Once you've ran `./tests.sh`, you can run `objdump -D tests_ok/helper_fn/results/expected.so -M intel` to display the assembler contents grug is expected to produce.

## Run tests.sh on save

1. Install this [Run on Save](https://marketplace.visualstudio.com/items?itemName=emeraldwalk.RunOnSave) extension for VS Code.
2. In the Output window, which is one of the buttons next to the Terminal button, press the dropdown on the right and select `Run On Save`. Here you will see the output of the tests every time you save.

## Generating coverage

> [!IMPORTANT]
> Make sure you have [gcovr](https://gcovr.com/en/stable/installation.html) (`pip install gcovr`) and [llvm-cov](https://llvm.org/docs/CommandGuide/llvm-cov.html) (`apt install llvm`)

Run `COVERAGE= ./tests.sh`, and view the generated `coverage.html` with your browser.

You should see that the program has nearly 100% line coverage.

## Fuzzing

This uses [libFuzzer](https://llvm.org/docs/LibFuzzer.html), which requires [Clang](https://en.wikipedia.org/wiki/Clang) to be installed.

If you replace `-fsanitize=address,undefined,fuzzer -Og` with `-fsanitize=fuzzer -Ofast -march=native -DNDEBUG` here, the numbers of executions per second goes up from 5000 to 15000 on my computer:

```bash
clear && \
clang grug/grug.c fuzz.c -Igrug -std=gnu2x -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wshadow -Wuninitialized -Wfatal-errors -Wno-unused-parameter -g -rdynamic -fsanitize=address,undefined,fuzzer -Og && \
mkdir -p test_corpus && \
for d in tests_err/* tests_err_runtime/* tests_ok/*; do name=${d##*/}; cp $d/input.grug test_corpus/$name.grug; done && \
mkdir -p corpus && \
./a.out -merge=1 corpus test_corpus && \
./a.out corpus -timeout=1
```

## Static code analysis with PVS-Studio

See [my blog post about PVS-Studio](https://mynameistrez.github.io/2024/08/19/static-c-analysis-with-pvs-studio.html) for the what, why, and how.

```bash
pvs-studio-analyzer trace -- clang run.c grug/grug.c -Igrug -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wshadow -Wuninitialized -Wfatal-errors -g -fsanitize=address,undefined -Og &&\
pvs-studio-analyzer analyze -o pvs.log &&\
plog-converter -a GA:1,2 -t json -o pvs.json pvs.log
```

## Linker map

In order to visualize what grug.c contains when linked, follow these steps:

1. Download [amap](https://www.sikorskiy.net/info/prj/amap/index.html)
2. Run `LINKER_MAP= ./tests.sh`
3. Run `./amap` in the cloned/unzipped directory of amap.
4. Click the `File` button in the top-left corner, and then `Open file`
5. Select the generated `output.map` using the file explorer popup

## Inspecting the tokens and AST

```bash
clang run.c grug/grug.c -Wall -Wextra -Werror -Wpedantic -Wshadow -Wfatal-errors -g -Igrug -fsanitize=address,undefined -DLOGGING && \
./a.out tests_ok/helper_fn/input.grug tests_ok/helper_fn/results/expected.so
```

## gdb

```bash
gdb --args a.out tests_ok/helper_fn/input.grug tests_ok/helper_fn/results/expected.so
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

## gdb on a test

```bash
nasm tests_ok/write_to_global_variable/input.s -f elf64 -o tests_ok/write_to_global_variable/results/expected.o &&\
ld -shared --hash-style=sysv tests_ok/write_to_global_variable/results/expected.o -o tests_ok/write_to_global_variable/results/expected.so &&\
rm tests_ok/write_to_global_variable/results/expected.o &&\
clang tests_ok/write_to_global_variable/test.c -Igrug -std=gnu2x -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wuninitialized -Wfatal-errors -g -Og -rdynamic -o tests_ok/write_to_global_variable/results/test &&\
gdb --args tests_ok/write_to_global_variable/results/test tests_ok/write_to_global_variable/results/expected.so
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
