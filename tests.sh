#!/bin/bash

compiler_flags='-Igrug -g -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wshadow -Wuninitialized -Wfatal-errors'

# This makes compilation quite a bit slower
# compiler_flags+=' -Og'

if [[ ${ASAN+x} ]]
then
    # This makes compilation quite a bit slower
    echo "- ASAN was turned on"
    compiler_flags+=' -fsanitize=address,undefined'
fi

compiler_flags+=' -DMODS_DIR_PATH="tests"'

if [[ ${OLD_LD+x} ]]
then
    echo "- OLD_LD was turned on"
    compiler_flags+=' -DOLD_LD'
fi

if [[ ${LOGGING+x} ]]
then
    echo "- LOGGING was turned on"
    compiler_flags+=' -DLOGGING'
fi

if [[ ${VALGRIND+x} ]]
then
    echo "- VALGRIND was turned on"
fi

if [ "$(uname)" == "Darwin" ]; then # If Mac OS X
    echo "Detected macOS"
    compiler_flags+=' -I.' # For `#include <elf.h>`
fi

# -rdynamic allows the .so to call functions from test.c
linker_flags='-rdynamic -lm'

CC="${CC:=clang}"
echo "Compilation will use $CC"

if [[ "$CC" = "gcc" ]]
then
    compiler_flags+=' -Wno-pragmas'
elif [[ "$CC" = "clang" ]]
then
    compiler_flags+=' -gdwarf-4' # build.yml requires this, for some reason
fi

if (! [[ grug/grug.c -ot grug.o ]]) || (! [[ grug/grug.h -ot grug.o ]]) || (! [[ tests.sh -ot grug.o ]])
then
    echo "Recompiling grug.o..."
    "$CC" grug/grug.c -c -o grug.o $compiler_flags || { echo 'Recompiling grug.o failed :('; exit 1; }
fi

if (! [[ tests.c -ot tests.o ]]) || (! [[ tests.sh -ot tests.o ]])
then
    echo "Recompiling tests.o..."
    "$CC" tests.c -c -o tests.o $compiler_flags || { echo 'Recompiling tests.o failed :('; exit 1; }
fi

if (! [[ tests.o -ot tests.out ]]) || (! [[ grug.o -ot tests.out ]]) || (! [[ tests.sh -ot tests.out ]])
then
    # TODO: Try using the mold linker here, and add README instructions back if it's faster
    echo "Linking tests.out..."
    # TODO: Try removing $compiler_flags here
    "$CC" tests.o grug.o -o tests.out $compiler_flags $linker_flags || { echo 'Linking tests.out failed :('; exit 1; }
fi

echo "Running tests.out..."
# "$@" passes any whitelisted test names to tests.out
if [[ ${VALGRIND+x} ]]
then
    # This makes compilation quite a bit slower
    valgrind --quiet ./tests.out "$@"
else
    ./tests.out "$@"
fi
