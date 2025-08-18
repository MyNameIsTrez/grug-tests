# grug tests

This is the test suite of [my programming language called grug](https://github.com/MyNameIsTrez/grug/).

## Running

[![CI Build](https://github.com/MyNameIsTrez/grug-tests/actions/workflows/build.yml/badge.svg)](https://github.com/MyNameIsTrez/grug-tests/actions/workflows/build.yml)

1. Clone this repository, and clone [grug](https://github.com/MyNameIsTrez/grug) next to it.
2. Run `cd grug-tests`.
3. Run the tests using VS Code's debugger by hitting F5, or running `./tests.sh ../grug`.

You can get more info in the test its generated `results` directory (from objdump, readelf and xxd) by running `OUTPUT_DLL_INFO= ./tests.sh ../grug`.

If a test failed, you can reproduce it by replacing `time(NULL)` in `main()` with the failing test's printed seed.

### Troubleshooting

If you're using a Debian-based distribution like Ubuntu 22.04, you might need to run `sudo sysctl vm.mmap_rnd_bits=28` to fix address sanitizer, which `tests.sh` uses. See [this GitHub thread](https://github.com/actions/runner-images/issues/9524#issuecomment-2002475952) for context.

If the tests don't pass, try running `OLD_LD= ./tests.sh ../grug`.

## Dependencies

`tests.sh` requires and has been tested with:

- [NASM version 2.15.05](https://launchpad.net/ubuntu/+source/nasm) (`nasm_2.15.05-1_amd64.deb` in my case)
- GNU ld (GNU Binutils for Ubuntu) 2.38
- [valgrind](https://valgrind.org/) (valgrind-3.18.1)

## More information

See the `development.md` file in this repository.
