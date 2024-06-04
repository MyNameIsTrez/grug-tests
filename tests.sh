#!/bin/sh

echo "Recompiling..."

gcc test.c grug/grug.c -Wall -Wextra -Werror -Wpedantic -Wfatal-errors -g -Igrug -Wno-misleading-indentation
if [ $? -ne 0 ]
then
	echo "Compilation failed"
	exit 1
fi

mkdir -p results/

expected_o_path="results/.intermediate.o"
expected_dll_path="results/expected.so"
dll_path="results/output.so"
c_path="results/output.c"
grug_output_path="results/grug_output.txt"
grug_hex_path="results/output.hex"
expected_hex_path="results/expected.hex"
error_diff_path="results/error_diff_path.txt"

run_test_err() {
	local dir=$1

	local grug_path="$dir/input.grug"

	printf "Running $dir...\n"

	./a.out $grug_path $dll_path >$grug_output_path 2>&1
	local grug_exit_status=$?

	local expected_error_path="$dir/expected_error.txt"

	diff $grug_output_path $expected_error_path >$error_diff_path

	if [ $? -ne 0 ]
	then
		echo "Differing outputs:" >&2
		cat $error_diff_path >&2
		exit 1
	fi

	if [ $grug_exit_status -eq 0 ]
	then
		echo "grug's test.c was expected to exit with status 1" >&2
		exit 1
	fi
}

run_tests_err() {
	# Loop over all directories in tests_err/ (non-recursively)
	for dir in tests_err/*/
	do
		run_test_err $dir
	done
}

run_test_ok() {
	local dir=$1

	local nasm_path="$dir/input.s"

	nasm -f elf64 $nasm_path -o $expected_o_path && ld -shared --hash-style=sysv $expected_o_path -o $expected_dll_path && rm $expected_o_path

	local grug_path="$dir/input.grug"

	printf "Running $dir...\n"

	./a.out $grug_path $dll_path $c_path >$grug_output_path 2>&1
	local grug_exit_status=$?

	if [ -s $grug_output_path ]
	then
		echo "The test wasn't supposed to print anything, but did:" >&2
		echo "----" >&2
		cat $grug_output_path >&2
		echo "----" >&2
		exit 1
	fi

	diff $dll_path $expected_dll_path >/dev/null

	if [ $? -ne 0 ]
	then
		xxd $dll_path > $grug_hex_path
		xxd $expected_dll_path > $expected_hex_path

		echo "The output differs from the expected output." >&2
		echo "Run this to see the diff:" >&2
		echo "diff $grug_hex_path $expected_hex_path" >&2

		exit 1
	fi

	if [ $grug_exit_status -ne 0 ]
	then
		echo "grug's test.c was expected to exit with status 0" >&2
		exit 1
	fi
}

run_tests_ok() {
	# Loop over all directories in tests_ok/ (non-recursively)
	for dir in tests_ok/*/
	do
		run_test_ok $dir
	done
}

# run_tests_err
run_tests_ok

echo "All tests passed!"
