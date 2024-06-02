#!/bin/sh

echo "Recompiling..."

gcc test.c grug/grug.c -Wall -Wextra -Werror -Wpedantic -Wfatal-errors -g -I. -Igrug -rdynamic -fsanitize=address,undefined -no-pie -DCONFIG_TRIPLET="\"x86_64-linux-gnu\"" -DTCC_TARGET_X86_64 -DONE_SOURCE=0 -DTCC_GITHASH="\"2024-03-03 mob@9d2068c6*\"" -fno-strict-aliasing -Wno-pointer-sign -Wno-sign-compare -Wno-unused-result -Wno-format-truncation -Wno-stringop-truncation -Wno-old-style-declaration -Wno-overlength-strings -Wno-implicit-fallthrough -Wno-missing-field-initializers -Wno-misleading-indentation

mkdir -p results/

expected_o_path="results/.intermediate.o"
expected_dll_path="results/expected.so"
dll_path="results/output.so"
grug_output_path="results/grug_output.txt"
grug_hex_path="results/output.hex"
expected_hex_path="results/expected.hex"

run_test() {
	local dir=$1

	nasm_path="$dir/input.s"

	nasm -f elf64 $nasm_path -o $expected_o_path && ld -shared --hash-style=sysv $expected_o_path -o $expected_dll_path && rm $expected_o_path

	grug_path="$dir/input.grug"

	printf "Running $dir...\n"

	./a.out $grug_path $dll_path 2>&1 >$grug_output_path

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

		echo "Differing outputs:" >&2
		diff $grug_hex_path $expected_hex_path >&2
		exit 1
	fi
}

run_tests() {
	# Loop over all directories in tests_ok/ (non-recursively)
	for dir in tests_ok/*/
	do
		run_test $dir
	done
}

run_tests

echo "All tests passed!"
