#!/bin/bash

ran_test_count=0

fail() {
	touch $1/results/failed
	exit 1
}

run_test_err() {
	local dir=$1

	ran_test_count=$((ran_test_count + 1))

	local grug_path=$dir"input.grug"
	local expected_error_path=$dir"expected_error.txt"
	local grug_output_path=$dir"results/grug_output.txt"

	if [[ $grug_output_path -nt $grug_path ]]\
	&& [[ $grug_output_path -nt $expected_error_path ]]\
	&& [[ $grug_output_path -nt mod_api.json ]]\
	&& [[ $grug_output_path -nt tests.sh ]]\
	&& [[ $grug_output_path -nt tests.out ]]\
	&& ! [[ -f $dir"/results/failed" ]]
	then
		printf "Skipping $dir...\n"
		return
	else
		printf "Running $dir...\n"

		[[ -f $dir"/results/failed" ]] && echo "  /results/failed caused this test to be rerun"

		! [[ $grug_output_path -nt $grug_path ]] && echo "  - input.grug was newer"
		! [[ $grug_output_path -nt $expected_error_path ]] && echo "  - expected_error.txt was newer"
		! [[ $grug_output_path -nt tests.out ]] && echo "  - tests.out was newer"
	fi

	mkdir -p $dir"results"
	rm -f $dir"results"/*

	local dll_path=$dir"results/output.so"

	printf "  Running tests.out...\n"
	./tests.out $grug_path $dll_path >$grug_output_path 2>&1
	local grug_exit_status=$?

	local error_diff_path=$dir"results/error_diff_path.txt"

	diff $grug_output_path $expected_error_path >$error_diff_path
	if [ $? -ne 0 ]
	then
		echo "The output differs from the expected output:" >&2
		cat $error_diff_path >&2
		failed=1
	fi

	if [ $grug_exit_status -eq 0 ]
	then
		echo "run.c was expected to exit with status 1" >&2
		failed=1
	fi

	if [ -n "$failed" ]
	then
		fail $dir
	fi
}

run_tests_err() {
	echo ERR tests:

	# Loop over all directories in tests_err/ (non-recursively)
	for dir in tests_err/*/
	do
		# The wildcard doesn't expand if the directory is empty
		[ -d "$dir" ] || continue

		run_test_err $dir
	done
}

run_test_err_runtime() {
	local dir=$1

	ran_test_count=$((ran_test_count + 1))

	local grug_path=$dir"input.grug"
	local expected_error_path=$dir"expected_error.txt"
	local test_c_path=$dir"test.c"
	local test_executable_path=$dir"results/test"
	local dll_path=$dir"results/output.so"

	if [[ $dll_path -nt $grug_path ]]\
	&& [[ $dll_path -nt $expected_error_path ]]\
	&& [[ $dll_path -nt $test_c_path ]]\
	&& [[ $dll_path -nt $test_executable_path ]]\
	&& [[ $dll_path -nt mod_api.h ]]\
	&& [[ $dll_path -nt mod_api.json ]]\
	&& [[ $dll_path -nt tests.sh ]]\
	&& [[ $dll_path -nt tests.out ]]\
	&& ! [[ -f $dir"/results/failed" ]]
	then
		printf "Skipping $dir...\n"
		return
	else
		printf "Running $dir...\n"

		[[ -f $dir"/results/failed" ]] && echo "  /results/failed caused this test to be rerun"

		! [[ $dll_path -nt $grug_path ]] && echo "  - input.grug was newer"
		! [[ $dll_path -nt $expected_error_path ]] && echo "  - expected_error.txt was newer"
		! [[ $dll_path -nt $test_c_path ]] && echo "  - test.c was newer"
		! [[ $dll_path -nt $test_executable_path ]] && echo "  - test was newer"
		! [[ $dll_path -nt tests.out ]] && echo "  - tests.out was newer"
	fi

	mkdir -p $dir"results"
	rm -f $dir"results"/*

	if ! [[ $test_c_path -ot $test_executable_path ]]
	then
		printf "  Recreating the executable 'test'...\n"

		# -std=gnu2x is used to have typeof() in C (-std=c2x for some reason prints "error: expected specifier-qualifier-list before ‘typeof’")
		# -rdynamic allows the .so to call functions from test.c
		clang $test_c_path grug.o -Igrug -I. -std=gnu2x -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wuninitialized -Wfatal-errors -Wno-language-extension-token -g -Og -rdynamic -o $test_executable_path
	fi

	local grug_output_path=$dir"results/grug_output.txt"

	printf "  Recreating output.so...\n"
	./tests.out $grug_path $dll_path >$grug_output_path 2>&1
	local grug_exit_status=$?

	if [ $grug_exit_status -ne 0 ]
	then
		echo "run.c was expected to exit with status 0" >&2
		failed=1
	fi

	if [ -s $grug_output_path ]
	then
		echo "The test wasn't supposed to print anything during generation of the dll, but did:" >&2
		echo "----" >&2
		cat $grug_output_path >&2
		echo "----" >&2
		failed=1
	fi

	local test_output_path=$dir"results/test_output_path.txt"

	printf "  Running the executable 'test'...\n"
	valgrind --quiet $test_executable_path $dll_path >$test_output_path 2>&1
	local test_exit_status=$?

	if [ $test_exit_status -eq 0 ]
	then
		echo "The shared object nasm produced wasn't supposed to pass test.c" >&2
		failed=1
	fi

	# Remove "can't grow stack" lines, since tests_err_runtime/stack_overflow tests that
	sed -i "/can't grow stack/d" "$test_output_path"

	# If the output still contains a valgrind line
	if grep -q "== " "$test_output_path"
	then
		echo "There was a valgrind error" >&2
		failed=1
	fi

	if [ $test_exit_status -eq 42 ]
	then
		echo "There was a valgrind error" >&2
		failed=1
	fi

	local error_diff_path=$dir"results/error_diff_path.txt"

	diff $test_output_path $expected_error_path >$error_diff_path
	if [ $? -ne 0 ]
	then
		echo "The output differs from the expected output:" >&2
		cat $error_diff_path >&2
		failed=1
	fi

	if [ -n "$failed" ]
	then
		if [ -s $dll_path ]
		then
			xxd $dll_path > $dir"results/output.hex"
			readelf -a $dll_path > $dir"results/output_elf.log"
			objdump -D $dll_path -M intel > $dir"results/output_objdump.log"
		fi

		fail $dir
	fi
}

run_tests_err_runtime() {
	echo ERR runtime tests:

	# Loop over all directories in tests_err_runtime/ (non-recursively)
	for dir in tests_err_runtime/*/
	do
		# The wildcard doesn't expand if the directory is empty
		[ -d "$dir" ] || continue

		run_test_err_runtime $dir
	done
}

run_test_ok() {
	local dir=$1

	ran_test_count=$((ran_test_count + 1))

	local nasm_path=$dir"input.s"
	local grug_path=$dir"input.grug"
	local expected_dll_path=$dir"results/expected.so"
	local test_c_path=$dir"test.c"
	local test_executable_path=$dir"results/test"
	local dll_path=$dir"results/output.so"

	if [[ $dll_path -nt $nasm_path ]]\
	&& [[ $dll_path -nt $grug_path ]]\
	&& [[ $dll_path -nt $expected_dll_path ]]\
	&& [[ $dll_path -nt $test_c_path ]]\
	&& [[ $dll_path -nt $test_executable_path ]]\
	&& [[ $dll_path -nt mod_api.h ]]\
	&& [[ $dll_path -nt mod_api.json ]]\
	&& [[ $dll_path -nt tests.sh ]]\
	&& [[ $dll_path -nt tests.out ]]\
	&& ! [[ -f $dir"/results/failed" ]]
	then
		printf "Skipping $dir...\n"
		return
	else
		printf "Running $dir...\n"

		[[ -f $dir"/results/failed" ]] && echo "  /results/failed caused this test to be rerun"

		! [[ $dll_path -nt $nasm_path ]] && echo "  - input.s was newer"
		! [[ $dll_path -nt $grug_path ]] && echo "  - input.grug was newer"
		! [[ $dll_path -nt $expected_dll_path ]] && echo "  - expected.so was newer"
		! [[ $dll_path -nt $test_c_path ]] && echo "  - test.c was newer"
		! [[ $dll_path -nt $test_executable_path ]] && echo "  - test was newer"
		! [[ $dll_path -nt tests.out ]] && echo "  - tests.out was newer"
	fi

	mkdir -p $dir"results"
	rm -f $dir"results"/*

	if ! [[ $nasm_path -ot $expected_dll_path ]]
	then
		printf "  Recreating expected.so...\n"

		local expected_o_path=$dir"results/expected.o"

		# -O0 turns optimizations off, which makes it easier to compare nasm's output against grug's
		nasm $nasm_path -f elf64 -O0 -o $expected_o_path && ld -shared --hash-style=sysv $expected_o_path -o $expected_dll_path && rm $expected_o_path

		# Rename the $nasm_path symbol to $grug_path, so the diff doesn't crap itself
		objcopy $expected_dll_path --redefine-sym $nasm_path=$grug_path
	fi

	if ! [[ $test_c_path -ot $test_executable_path ]]
	then
		printf "  Recreating the executable 'test'...\n"

		# -std=gnu2x is used to have typeof() in C (-std=c2x for some reason prints "error: expected specifier-qualifier-list before ‘typeof’")
		# -rdynamic allows the .so to call functions from test.c
		clang $test_c_path grug.o -Igrug -I. -std=gnu2x -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wuninitialized -Wfatal-errors -Wno-language-extension-token -g -Og -rdynamic -lm -o $test_executable_path
	fi

	printf "  Running the executable 'test'...\n"
	valgrind --error-exitcode=1 --quiet $test_executable_path $expected_dll_path
	local test_exit_status=$?

	if [ $test_exit_status -ne 0 ]
	then
		echo "The shared object nasm produced didn't pass test.c" >&2
		failed=1
	fi

	local grug_output_path=$dir"results/grug_output.txt"

	printf "  Recreating output.so...\n"
	./tests.out $grug_path $dll_path >$grug_output_path 2>&1
	local grug_exit_status=$?

	if [ -s $grug_output_path ]
	then
		echo "The test wasn't supposed to print anything, but did:" >&2
		echo "----" >&2
		cat $grug_output_path >&2
		echo "----" >&2
		failed=1
	fi

	local error_diff_path=$dir"results/error_diff_path.txt"

	diff $dll_path $expected_dll_path >$error_diff_path
	if [ $? -ne 0 ]
	then
		echo "The output differs from the expected output." >&2
		cat $error_diff_path >&2
		failed=1
	fi

	if [ $grug_exit_status -ne 0 ]
	then
		echo "run.c was expected to exit with status 0" >&2
		failed=1
	fi

	if [ -n "$failed" ]
	then
		if [ -s $expected_dll_path ]
		then
			xxd $expected_dll_path > $dir"results/expected.hex"
			readelf -a $expected_dll_path > $dir"results/expected_elf.log"
			objdump -D $expected_dll_path -M intel > $dir"results/expected_objdump.log"
		fi

		if [ -s $dll_path ]
		then
			xxd $dll_path > $dir"results/output.hex"
			readelf -a $dll_path > $dir"results/output_elf.log"
			objdump -D $dll_path -M intel > $dir"results/output_objdump.log"
		fi

		fail $dir
	fi
}

run_tests_ok() {
	echo OK tests:

	# Loop over all directories in tests_ok/ (non-recursively)
	for dir in tests_ok/*/
	do
		# The wildcard doesn't expand if the directory is empty
		[ -d "$dir" ] || continue

		run_test_ok $dir
	done
}

init() {
	# We remove all .gcda files here, because if you place say a newline in a function of grug.c
	# the generated .gcda will have a different checksum, causing a gcovr error
	# The .gcno is only generated during compilation, so we don't remove it
	rm -f *.gcda

	if (! [[ run.c -ot tests.out ]]) || (! [[ grug/grug.c -ot tests.out ]]) || (! [[ grug/grug.h -ot tests.out ]])
	then
		echo "Recompiling..."

		local compiler_flags='-Igrug -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wshadow -Wuninitialized -Wfatal-errors -g'
		local linker_flags=''

		# TODO: An issue here is that if LOGGING is set or unset, tests.out isn't recompiled!
		# TODO: This could also definitely be done inline with some sort of ternary
		if [[ -v LOGGING ]] # If the LOGGING environment variable was set
		then
		    echo "- LOGGING was turned on"
			compiler_flags+=' -DLOGGING'
		fi

		# TODO: An issue here is that if OPTIMIZED is set or unset, tests.out isn't recompiled!
		# TODO: This could also definitely be done inline with some sort of ternary
		if [[ -v OPTIMIZED ]] # If the OPTIMIZED environment variable was set
		then
		    echo "- OPTIMIZED was turned on"
		fi

		# TODO: An issue here is that if OLD_LD is set or unset, tests.out isn't recompiled!
		# TODO: This could also definitely be done inline with some sort of ternary
		if [[ -v OLD_LD ]] # If the OLD_LD environment variable was set
		then
		    echo "- OLD_LD was turned on"
			compiler_flags+=' -DOLD_LD'
		fi

		# TODO: An issue here is that if CRASH_ON_UNREACHABLE is set or unset, tests.out isn't recompiled!
		# TODO: This could also definitely be done inline with some sort of ternary
		if [[ -v CRASH_ON_UNREACHABLE ]] # If the CRASH_ON_UNREACHABLE environment variable was set
		then
		    echo "- CRASH_ON_UNREACHABLE was turned on"
			compiler_flags+=' -DCRASH_ON_UNREACHABLE'
		fi

		# TODO: An issue here is that if COVERAGE is set or unset, tests.out isn't recompiled!
		# TODO: This could also definitely be done inline with some sort of ternary
		if [[ -v COVERAGE ]] # If the COVERAGE environment variable was set
		then
		    echo "- COVERAGE was turned on"
			compiler_flags+=' --coverage'
		fi

		# TODO: An issue here is that if LINKER_MAP is set or unset, tests.out isn't recompiled!
		# TODO: This could also definitely be done inline with some sort of ternary
		if [[ -v LINKER_MAP ]] # If the LINKER_MAP environment variable was set
		then
		    echo "- LINKER_MAP was turned on"
			linker_flags+=' -Wl,-Map=output.map'
		fi

		clang grug/grug.c -c -o grug.o $compiler_flags

		# TODO: An issue here is that if OPTIMIZED is set or unset, tests.out isn't recompiled!
		# TODO: This could also definitely be done inline with some sort of ternary
		if [[ -v OPTIMIZED ]] # If the OPTIMIZED environment variable was set
		then
			compiler_flags+=' -Ofast -march=native -DNDEBUG'
        else
			compiler_flags+=' -fsanitize=address,undefined -Og'
		fi

		clang run.c grug/grug.c -o tests.out $compiler_flags $linker_flags

		if [ $? -ne 0 ]
		then
			echo "Compilation failed"
			exit 1
		fi

		echo
	fi
}

usage() {
	printf "Usage: %s [test_path]\n" $0
	exit 1
}

if [[ $# > 1 ]]
then
	usage
elif [[ $1 == "" ]]
then
	init
	run_tests_err
	echo
	run_tests_err_runtime
	echo
	run_tests_ok
elif [[ $1 == tests_err/* ]]
then
	init
	run_test_err $1/
elif [[ $1 == tests_err ]]
then
	init
	run_tests_err
elif [[ $1 == tests_err_runtime/* ]]
then
	init
	run_test_err_runtime $1/
elif [[ $1 == tests_err_runtime ]]
then
	init
	run_tests_err_runtime
elif [[ $1 == tests_ok/* ]]
then
	init
	run_test_ok $1/
elif [[ $1 == tests_ok ]]
then
	init
	run_tests_ok
else
	usage
fi

printf "\nAll $ran_test_count tests passed!\n"

if [[ -v COVERAGE ]]
then
	gcovr --gcov-executable "llvm-cov gcov" --html-details coverage.html
fi
