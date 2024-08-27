#define _XOPEN_SOURCE 700

#include "grug/grug.h"

#include "mod_api.h"

#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void check(int status, char *fn_name) {
	if (status < 0) {
		perror(fn_name);
		exit(EXIT_FAILURE);
	}
}

static void check_null(void *ptr, char *fn_name) {
	if (ptr == NULL) {
		perror(fn_name);
		exit(EXIT_FAILURE);
	}
}

void game_fn_define_d(void) {}

void game_fn_initialize(int32_t x) {
	(void)x;
}

// #define OK_PARSE(path, node) {\
// 	assert(!json(path, node) || (\
// 			fprintf(\
// 				stderr,\
// 				"json.c:%d: %s in %s\n",\
// 				json_error_line_number,\
// 				json_error_messages[json_error],\
// 				path\
// 			), abort(), false));\
// }

// #define ERROR_PARSE(path, error) {\
// 	struct json_node node;\
// 	assert(json(path, &node));\
// 	assert(json_error == error);\
// }

// static void ok_string_foo(void) {
// 	struct json_node node;
// 	OK_PARSE("./tests_ok/string_foo.json", &node);
// 	assert(node.type == JSON_NODE_STRING);
// 	assert(strcmp(node.data.string, "foo") == 0);
// }

// static void error_expected_array_close(void) {
// 	ERROR_PARSE("./tests_err/expected_array_close.json", JSON_ERROR_EXPECTED_ARRAY_CLOSE);
// }

static void *get(void *handle, char *label) {
	void *p = dlsym(handle, label);
	if (!p) {
		printf("dlsym: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}
	return p;
}

static char *get_expected_error(char *expected_error_path) {
	// TODO: Check if using mmap() makes this faster:
	// https://stackoverflow.com/a/174808/13279557
	FILE *f = fopen(expected_error_path, "r");
	check_null(f, "fopen");

	check(fseek(f, 0, SEEK_END), "fseek");

	long ftell_result = ftell(f);
	check(ftell_result, "ftell");
	size_t expected_error_len = ftell_result;

	static char expected_error[420];

	check(fseek(f, 0, SEEK_SET), "fseek");

	if (fread(expected_error, expected_error_len, 1, f) == 0) {
		if (feof(f)) {
			printf("fread EOF\n");
		}
		if (ferror(f)) {
			printf("fread error\n");
		}
		exit(EXIT_FAILURE);
	}

	if (expected_error[expected_error_len - 1] == '\n') {
		expected_error_len--;
		if (expected_error[expected_error_len - 1] == '\r') {
			expected_error_len--;
		}
	}

	expected_error[expected_error_len] = '\0';

    if (fclose(f) == EOF) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}

	return expected_error;
}

static void compare_against_expected_error(char *msg, char *expected_error_path) {
	size_t grug_error_msg_len = strlen(msg);

	char *expected_error = get_expected_error(expected_error_path);
	size_t expected_error_len = strlen(expected_error);

	if (expected_error_len != grug_error_msg_len || memcmp(msg, expected_error, expected_error_len) != 0) {
		printf("The output differs from the expected output.\n");
		printf("Output:\n");
		printf("%s\n", msg);

		printf("Expected:\n");
		printf("%s\n", expected_error);

		exit(EXIT_FAILURE);
	}
}

static void error_assignment_isnt_expression(void) {
	printf("Running error_assignment_isnt_expression...\n");

	char *grug_path = "tests_err/assign_to_unknown_variable/input.grug";
	char *expected_error_path = "tests_err/assign_to_unknown_variable/expected_error.txt";
	char *results_path = "tests_err/assign_to_unknown_variable/results";
	char *output_dll_path = "tests_err/assign_to_unknown_variable/results/output.so";

	if (mkdir(results_path, 0755) == -1 && errno != EEXIST) {
		perror("mkdir");
		exit(EXIT_FAILURE);
	}

	assert(grug_test_regenerate_dll(grug_path, output_dll_path));

	compare_against_expected_error(grug_error.msg, expected_error_path);
}

static void runtime_error_division_by_0(void) {
	printf("Running runtime_error_division_by_0...\n");

	char *grug_path = "tests_err_runtime/division_by_0/input.grug";
	char *expected_error_path = "tests_err_runtime/division_by_0/expected_error.txt";
	char *results_path = "tests_err_runtime/division_by_0/results";
	char *output_dll_path = "tests_err_runtime/division_by_0/results/output.so";

	if (mkdir(results_path, 0755) == -1 && errno != EEXIST) {
		perror("mkdir");
		exit(EXIT_FAILURE);
	}

	if (grug_test_regenerate_dll(grug_path, output_dll_path)) {
		printf("The test wasn't supposed to print anything during generation of the dll, but did:\n");
		printf("----\n");
		printf("%s\n", grug_error.msg);
		printf("----\n");

		exit(EXIT_FAILURE);
	}

	void *handle = dlopen(output_dll_path, RTLD_NOW);
	if (!handle) {
		printf("dlopen: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	assert(strcmp(get(handle, "define_type"), "d") == 0);

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	grug_define_fn_t define = get(handle, "define");
	define();

	size_t globals_size = *(size_t *)get(handle, "globals_size");
	assert(globals_size == 0);

	void *g = malloc(globals_size);
	grug_init_globals_fn_t init_globals = get(handle, "init_globals");
	init_globals(g);

	struct d_on_fns *on_fns = get(handle, "on_fns");
	#pragma GCC diagnostic pop

	bool had_runtime_error = false;

	if (grug_mod_had_runtime_error()) {
		had_runtime_error = true;
	}

	if (!had_runtime_error) {
		on_fns->a(g);
	}

	free(g);

	assert(had_runtime_error);

	char *grug_error_msg = grug_get_runtime_error_reason();

	compare_against_expected_error(grug_error_msg, expected_error_path);
}

static void ok_addition_as_argument(void) {
	printf("Running ok_addition_as_argument...\n");

	char *grug_path = "tests_ok/addition_as_argument/input.grug";
	char *nasm_path = "tests_ok/addition_as_argument/input.s";
	char *results_path = "tests_ok/addition_as_argument/results";
	// char *output_dll_path = "tests_ok/addition_as_argument/results/output.so";
	char *expected_dll_path = "tests_ok/addition_as_argument/results/expected.so";
	char *nasm_o_path = "tests_ok/addition_as_argument/results/expected.o";

	if (mkdir(results_path, 0755) == -1 && errno != EEXIST) {
		perror("mkdir");
		exit(EXIT_FAILURE);
	}

	struct stat nasm_stat;
	check(stat(nasm_path, &nasm_stat), "stat");

	bool needs_regeneration = false;
	struct stat expected_dll_stat;
	if (stat(expected_dll_path, &expected_dll_stat) == -1) {
		if (errno != ENOENT) {
			perror("stat");
			exit(EXIT_FAILURE);
		}
		needs_regeneration = true;
	} else if (nasm_stat.st_mtime > expected_dll_stat.st_mtime) {
		needs_regeneration = true;
	}

	if (needs_regeneration) {
		printf("  Recreating expected.so...\n");

		pid_t pid = fork();
		check(pid, "fork");

		if (pid == 0) {
			check(execvp("nasm", (char *[]){"nasm", nasm_path, "-felf64", "-O0", "-o", nasm_o_path, NULL}), "execvp");
		}

		// Wait on the nasm child to finish
		check(wait(NULL), "wait");

		pid = fork();
		check(pid, "fork");

		if (pid == 0) {
			check(execvp("ld", (char *[]){"ld", nasm_o_path, "-o", expected_dll_path, "-shared", "--hash-style=sysv", NULL}), "execvp");
		}

		// Wait on the ld child to finish
		check(wait(NULL), "wait");

		remove(nasm_o_path);

		pid = fork();
		check(pid, "fork");

		if (pid == 0) {
			static char redefine_sym[420];

			size_t nasm_path_len = strlen(nasm_path);

			strcpy(redefine_sym, nasm_path);
			redefine_sym[nasm_path_len] = '=';
			strcpy(redefine_sym + nasm_path_len + 1, grug_path);

			check(execvp("objcopy", (char *[]){"objcopy", expected_dll_path, "--redefine-sym", redefine_sym, NULL}), "execvp");
		}

		// Wait on the objcopy child to finish
		check(wait(NULL), "wait");
	}
}

static void error_tests(void) {
	error_assignment_isnt_expression();
	// error_assign_to_unknown_variable();
	// error_bool_unary_minus();
	// error_cant_add_strings();
	// error_cant_call_define_fn_1();
	// error_cant_call_define_fn_2();
	// error_cant_redefine_global();
	// error_define_fn_calls_fn();
	// error_define_fn_different_name();
	// error_define_fn_not_enough_arguments();
	// error_define_fn_only_one_max();
	// error_define_fn_uses_global_variable();
	// error_define_fn_was_not_declared();
	// error_f32_missing_digit_after_decimal_point();
	// error_game_fn_does_not_exist();
	// error_game_function_call_gets_wrong_arg_type();
	// error_game_function_call_less_args_expected();
	// error_game_function_call_more_args_expected();
	// error_game_function_call_no_args_expected();
	// error_global_variable_already_uses_local_variable_name();
	// error_global_variable_before_define();
	// error_global_variable_calls_fn();
	// error_global_variable_definition_cant_use_itself();
	// error_global_variable_definition_requires_value_i32();
	// error_global_variable_definition_requires_value_string();
	// error_global_variable_uses_global_variable();
	// error_helper_fn_does_not_exist();
	// error_helper_function_call_gets_wrong_arg_type();
	// error_helper_function_call_less_args_expected();
	// error_helper_function_call_more_args_expected();
	// error_helper_function_call_no_args_expected();
	// error_helper_function_different_return_value_expected();
	// error_helper_function_missing_return_statement();
	// error_helper_function_no_return_value_expected();
	// error_i32_logical_not();
	// error_i32_too_big();
	// error_i32_too_small();
	// error_local_variable_already_exists();
	// error_local_variable_definition_cant_use_itself();
	// error_missing_define_fn();
	// error_no_space_between_comment_character_and_comment();
	// error_on_fn_before_define();
	// error_on_fn_duplicate();
	// error_on_fn_was_not_declared_in_entity();
	// error_on_fn_wrong_order();
	// error_on_function_gets_wrong_arg_type();
	// error_on_function_less_args_expected();
	// error_on_function_more_args_expected();
	// error_on_function_no_args_expected();
	// error_on_function_no_return_value_expected();
	// error_pass_bool_to_i32_game_param();
	// error_pass_bool_to_i32_helper_param();
	// error_resource_type_for_global();
	// error_resource_type_for_helper_fn_argument();
	// error_resource_type_for_helper_fn_return_type();
	// error_resource_type_for_local();
	// error_resource_type_for_on_fn_argument();
	// error_string_pointer_arithmetic();
	// error_too_many_f32_arguments();
	// error_too_many_i32_arguments();
	// error_trailing_space_in_comment();
	// error_unclosed_double_quote();
	// error_unknown_variable();
	// error_unused_result();
	// error_variable_assignment_before_definition();
	// error_variable_definition_requires_value_i32();
	// error_variable_definition_requires_value_string();
	// error_variable_statement_missing_assignment();
	// error_variable_used_before_definition();
	// error_wrong_type_global_assignment();
	// error_wrong_type_global_reassignment();
	// error_wrong_type_local_assignment();
	// error_wrong_type_local_reassignment();
}

static void runtime_error_tests(void) {
	runtime_error_division_by_0();
	// runtime_error_raise_alrm_with_handler();
	// runtime_error_raise_fpe_with_handler();
	// runtime_error_raise_sigsegv_with_handler();
	// runtime_error_stack_overflow();
	// runtime_error_time_limit_exceeded();
}

static void ok_tests(void) {
	ok_addition_as_argument();
	// ok_addition_as_two_arguments();
	// ok_addition_i32_wraparound();
	// ok_addition_with_multiplication();
	// ok_addition_with_multiplication_2();
	// ok_and_false_1();
	// ok_and_false_2();
	// ok_and_false_3();
	// ok_and_short_circuit();
	// ok_and_true();
	// ok_blocked_alrm();
	// ok_bool_logical_not_false();
	// ok_bool_logical_not_true();
	// ok_bool_returned();
	// ok_break();
	// ok_calls_100();
	// ok_calls_1000();
	// ok_continue();
	// ok_define_containing_addition();
	// ok_define_containing_string();
	// ok_define_with_eight_f32_fields();
	// ok_define_with_six_fields();
	// ok_define_with_six_i32_fields();
	// ok_define_with_six_string_fields();
	// ok_division_negative_result();
	// ok_division_positive_result();
	// ok_else_false();
	// ok_else_true();
	// ok_eq_false();
	// ok_eq_true();
	// ok_f32_addition();
	// ok_f32_argument();
	// ok_f32_division();
	// ok_f32_eq_false();
	// ok_f32_eq_true();
	// ok_f32_ge_false();
	// ok_f32_ge_true_1();
	// ok_f32_ge_true_2();
	// ok_f32_global_variable();
	// ok_f32_gt_false();
	// ok_f32_gt_true();
	// ok_f32_le_false();
	// ok_f32_le_true_1();
	// ok_f32_le_true_2();
	// ok_f32_local_variable();
	// ok_f32_lt_false();
	// ok_f32_lt_true();
	// ok_f32_multiplication();
	// ok_f32_ne_false();
	// ok_f32_negated();
	// ok_f32_ne_true();
	// ok_f32_passed_to_helper_fn();
	// ok_f32_passed_to_on_fn();
	// ok_f32_passing_sin_to_cos();
	// ok_f32_subtraction();
	// ok_fibonacci();
	// ok_ge_false();
	// ok_ge_true_1();
	// ok_ge_true_2();
	// ok_global_containing_addition();
	// ok_globals();
	// ok_globals_1000_i32();
	// ok_globals_32();
	// ok_globals_64();
	// ok_gt_false();
	// ok_gt_true();
	// ok_helper_fn();
	// ok_helper_fn_overwriting_param();
	// ok_helper_fn_returning_void_has_no_return();
	// ok_helper_fn_returning_void_returns_void();
	// ok_i32_max();
	// ok_i32_min();
	// ok_i32_negated();
	// ok_identical_strings_are_shared();
	// ok_if_false();
	// ok_if_true();
	// ok_le_false();
	// ok_le_true_1();
	// ok_le_true_2();
	// ok_lt_false();
	// ok_lt_true();
	// ok_max_args();
	// ok_minimal();
	// ok_multiplication_as_two_arguments();
	// ok_ne_false();
	// ok_negate_parenthesized_expr();
	// ok_negative_literal();
	// ok_nested_break();
	// ok_nested_continue();
	// ok_ne_true();
	// ok_no_define_fields();
	// ok_no_on_fns();
	// ok_on_fn();
	// ok_on_fn_calling_game_fn_nothing();
	// ok_on_fn_calling_game_fn_nothing_twice();
	// ok_on_fn_calling_game_fn_plt_order();
	// ok_on_fn_calling_helper_fns();
	// ok_on_fn_overwriting_param();
	// ok_on_fn_passing_argument_to_helper_fn();
	// ok_on_fn_passing_magic_to_initialize();
	// ok_on_fn_three();
	// ok_on_fn_three_unused_first();
	// ok_on_fn_three_unused_second();
	// ok_on_fn_three_unused_third();
	// ok_or_false();
	// ok_or_short_circuit();
	// ok_or_true_1();
	// ok_or_true_2();
	// ok_or_true_3();
	// ok_pass_string_argument_to_game_fn();
	// ok_pass_string_argument_to_helper_fn();
	// ok_remainder_negative_result();
	// ok_remainder_positive_result();
	// ok_resource_in_define();
	// ok_return();
	// ok_return_from_on_fn();
	// ok_return_with_no_value();
	// ok_stack_16_byte_alignment();
	// ok_stack_16_byte_alignment_midway();
	// ok_string_and_on_fn();
	// ok_string_eq_false();
	// ok_string_eq_true();
	// ok_string_eq_true_empty();
	// ok_string_ne_false();
	// ok_string_ne_false_empty();
	// ok_string_ne_true();
	// ok_subtraction_negative_result();
	// ok_subtraction_positive_result();
	// ok_variable();
	// ok_variable_does_not_shadow_define_fn();
	// ok_variable_reassignment();
	// ok_variable_shadows_define_fn();
	// ok_variable_shadows_game_fn();
	// ok_variable_shadows_helper_fn();
	// ok_variable_shadows_on_fn_1();
	// ok_variable_shadows_on_fn_2();
	// ok_variable_string_global();
	// ok_variable_string_local();
	// ok_void_function_early_return();
	// ok_while_false();
	// ok_write_to_global_variable();
}

int main(void) {
	error_tests();
	runtime_error_tests();
	ok_tests();
}
