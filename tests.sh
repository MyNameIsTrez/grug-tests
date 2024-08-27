#!/bin/bash

compiler_flags='-Igrug -g -Og -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wshadow -Wuninitialized -Wfatal-errors'

# -rdynamic allows the .so to call functions from test.c
linker_flags='-rdynamic'

# TODO: Consider always outputting and running grug_asan.o and grug_valgrind.o
# compiler_flags+=' -fsanitize=address,undefined'

if (! [[ grug/grug.c -ot tests.out ]]) || (! [[ grug/grug.h -ot tests.out ]]) || (! [[ tests.sh -ot tests.out ]])
then
    echo "Recompiling grug.o..."
    clang grug/grug.c -c -o grug.o $compiler_flags
fi

if (! [[ tests.c -ot tests.o ]]) || (! [[ tests.sh -ot tests.out ]])
then
    echo "Recompiling tests.o..."
    clang tests.c -c -o tests.o $compiler_flags
fi

# TODO: Try using the mold linker here, and add README instructions back if it's faster
echo "Linking tests.out..."
# TODO: Try removing $compiler_flags here
clang tests.o grug.o -o tests.out $compiler_flags $linker_flags

echo "Running tests.out..."
valgrind --quiet ./tests.out
# ./tests.out
