# grug tests

This is the test suite of [my programming language called grug](https://github.com/MyNameIsTrez/grug/).

## Running

1. Clone this repository and `cd` into it.
2. Run `git submodule update --init` to clone the `grug.c` and `grug.h` files (for your own game you can just drop these files directly into your project).
3. Run the tests with `./tests.sh`.

Check the terminal or the generated `results` directory in tests to inspect any errors.

### Troubleshooting

If you're using a Debian-based distribution like Ubuntu 22.04, you might need to run `sudo sysctl vm.mmap_rnd_bits=28` to fix address sanitizer, which `tests.sh` uses. See [this GitHub thread](https://github.com/actions/runner-images/issues/9524#issuecomment-2002475952) for context.

If the tests don't pass, try running `OLD_LD= ./tests.sh`.

## Dependencies

`tests.sh` requires and has been tested with:

- [NASM version 2.15.05](https://launchpad.net/ubuntu/+source/nasm) (`nasm_2.15.05-1_amd64.deb` in my case)
- GNU ld (GNU Binutils for Ubuntu) 2.38
- [valgrind](https://valgrind.org/) (valgrind-3.18.1)
