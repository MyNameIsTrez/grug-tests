#!/bin/bash

passed_test_count=0

fail() {
	touch $1/results/failed
	exit 1
}

run_test_err() {
	local dir=$1

	local grug_path=$dir"input.grug"
	local expected_error_path=$dir"expected_error.txt"
	local grug_output_path=$dir"results/grug_output.txt"

	if [[ $grug_output_path -nt $grug_path ]]\
	&& [[ $grug_output_path -nt $expected_error_path ]]\
	&& [[ $grug_output_path -nt mod_api.json ]]\
	&& [[ $grug_output_path -nt tests.sh ]]\
	&& [[ $grug_output_path -nt a.out ]]\
	&& ! [[ -f $dir"/results/failed" ]]
	then
		printf "Skipping $dir...\n"
		return
	else
		printf "Running $dir...\n"

		[[ -f $dir"/results/failed" ]] && echo "  /results/failed caused this test to be rerun"

		! [[ $grug_output_path -nt $grug_path ]] && echo "  - input.grug was newer"
		! [[ $grug_output_path -nt $expected_error_path ]] && echo "  - expected_error.txt was newer"
		! [[ $grug_output_path -nt a.out ]] && echo "  - a.out was newer"
	fi

	mkdir -p $dir"results"
	rm -f $dir"results"/*

	local dll_path=$dir"results/output.so"

	printf "  Running a.out...\n"
	./a.out $grug_path $dll_path >$grug_output_path 2>&1
	local grug_exit_status=$?

	local error_diff_path=$dir"results/error_diff_path.txt"

	diff $grug_output_path $expected_error_path >$error_diff_path

	if [ $? -ne 0 ]
	then
		echo "Differing outputs:" >&2
		cat $error_diff_path >&2
		fail $dir
	fi

	if [ $grug_exit_status -eq 0 ]
	then
		echo "run.c was expected to exit with status 1" >&2
		fail $dir
	fi

	passed_test_count=$((passed_test_count + 1))
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

run_test_ok() {
	local dir=$1

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
	&& [[ $dll_path -nt mod_api.json ]]\
	&& [[ $dll_path -nt tests.sh ]]\
	&& [[ $dll_path -nt a.out ]]\
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
		! [[ $dll_path -nt a.out ]] && echo "  - a.out was newer"
	fi

	mkdir -p $dir"results"
	rm -f $dir"results"/*

	if ! [[ $nasm_path -ot $expected_dll_path ]]
	then
		printf "  Recreating expected.so...\n"

		local expected_o_path=$dir"results/intermediate.o"

		nasm $nasm_path -f elf64 -O0 -o $expected_o_path && ld -shared --hash-style=sysv $expected_o_path -o $expected_dll_path && rm $expected_o_path

		# Rename the $nasm_path symbol to $grug_path, so the diff doesn't crap itself
		objcopy $expected_dll_path --redefine-sym $nasm_path=$grug_path
	fi

	if ! [[ $test_c_path -ot $test_executable_path ]]
	then
		printf "  Recreating the executable 'test'...\n"

		# -std=gnu2x is used to have typeof() in C (-std=c2x for some reason prints "error: expected specifier-qualifier-list before ‘typeof’")
		# -rdynamic allows the .so to call functions from test.c
		gcc $test_c_path -Igrug -std=gnu2x -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wfatal-errors -rdynamic -g -o $test_executable_path
	fi

	$test_executable_path $expected_dll_path
	if [ $? -ne 0 ]
	then
		echo "The shared object nasm produced didn't pass test.c" >&2
		fail $dir
	fi

	local grug_output_path=$dir"results/grug_output.txt"

	printf "  Recreating output.so...\n"
	./a.out $grug_path $dll_path >$grug_output_path 2>&1
	local grug_exit_status=$?

	if [ -s $grug_output_path ]
	then
		echo "The test wasn't supposed to print anything, but did:" >&2
		echo "----" >&2
		cat $grug_output_path >&2
		echo "----" >&2
		fail $dir
	fi

	diff $dll_path $expected_dll_path >/dev/null

	if [ $? -ne 0 ]
	then
		local grug_hex_path=$dir"results/output.hex"
		local expected_hex_path=$dir"results/expected.hex"

		xxd $dll_path > $grug_hex_path
		xxd $expected_dll_path > $expected_hex_path

		echo "The output differs from the expected output." >&2
		echo "Run this to see the diff:" >&2
		echo "diff $grug_hex_path $expected_hex_path" >&2

		readelf -a $dll_path > $dir"results/output_elf.hex"
		readelf -a $expected_dll_path > $dir"results/expected_elf.hex"

		objdump -D $dll_path > $dir"results/output_objdump.log"
		objdump -D $expected_dll_path > $dir"results/expected_objdump.log"

		fail $dir
	fi

	if [ $grug_exit_status -ne 0 ]
	then
		echo "run.c was expected to exit with status 0" >&2
		fail $dir
	fi

	passed_test_count=$((passed_test_count + 1))
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
	if (! [[ run.c -ot a.out ]]) || (! [[ grug/grug.c -ot a.out ]]) || (! [[ grug/grug.h -ot a.out ]])
	then
		echo "Recompiling..."

		# TODO: An issue here is that if LOGGING is set or unset, a.out isn't recompiled!
		# TODO: This could also definitely be done inline with some sort of ternary
		local logging_flag=''
		if [[ -v LOGGING ]] # If the LOGGING environment variable was set
		then
			logging_flag=' -DLOGGING'
		fi

		gcc run.c grug/grug.c -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -Wshadow -Wfatal-errors -g -Igrug -fsanitize=address,undefined $logging_flag

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
	run_tests_ok
elif [[ $1 == tests_err/* ]]
then
	init
	run_test_err $1/
elif [[ $1 == tests_ok/* ]]
then
	init
	run_test_ok $1/
else
	usage
fi

printf "\nAll $passed_test_count tests passed!\n"
