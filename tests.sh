#!/bin/bash

compiler_flags='-Igrug -g -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wshadow -Wuninitialized -Wfatal-errors'

# This makes compilation quite a bit slower
# compiler_flags+=' -Og'

# This makes compilation quite a bit slower
# compiler_flags+=' -fsanitize=address,undefined'

compiler_flags+=' -DMODS_DIR_PATH="tests"'

if [[ -v OLD_LD ]] # If the OLD_LD environment variable was set
then
    echo "- OLD_LD was turned on"
    compiler_flags+=' -DOLD_LD'
fi

# -rdynamic allows the .so to call functions from test.c
linker_flags='-rdynamic -lm'

if (! [[ grug/grug.c -ot grug.o ]]) || (! [[ grug/grug.h -ot grug.o ]]) || (! [[ tests.sh -ot grug.o ]])
then
    echo "Recompiling grug.o..."
    clang grug/grug.c -c -o grug.o $compiler_flags || { echo 'Recompiling grug.o failed :('; exit 1; }
fi

if (! [[ tests.c -ot tests.o ]]) || (! [[ tests.sh -ot tests.o ]])
then
    echo "Recompiling tests.o..."
    clang tests.c -c -o tests.o $compiler_flags || { echo 'Recompiling tests.o failed :('; exit 1; }
fi

if (! [[ tests.o -ot tests.out ]]) || (! [[ grug.o -ot tests.out ]]) || (! [[ tests.sh -ot tests.out ]])
then
    # TODO: Try using the mold linker here, and add README instructions back if it's faster
    echo "Linking tests.out..."
    # TODO: Try removing $compiler_flags here
    clang tests.o grug.o -o tests.out $compiler_flags $linker_flags || { echo 'Linking tests.out failed :('; exit 1; }
fi

echo "Running tests.out..."
# "$@" passes any whitelisted test names to tests.out
# valgrind --quiet ./tests.out "$@"
./tests.out "$@"
