#define _XOPEN_SOURCE 700

#include "grug/grug.h"

#include "mod_api.h"

#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

struct runtime_error_data {
	bool run;
	void *on_fns;
	void *g;
};

static size_t game_fn_nothing_call_count;
static size_t game_fn_magic_call_count;
static size_t game_fn_initialize_call_count;
static size_t game_fn_initialize_bool_call_count;
static size_t game_fn_identity_call_count;
static size_t game_fn_max_call_count;
static size_t game_fn_say_call_count;
static size_t game_fn_sin_call_count;
static size_t game_fn_cos_call_count;
static size_t game_fn_mega_call_count;
static size_t game_fn_is_friday_call_count;
static size_t game_fn_set_is_happy_call_count;
static size_t game_fn_mega_f32_call_count;
static size_t game_fn_mega_i32_call_count;

static int32_t game_fn_initialize_x;

// void game_fn_nothing() {
// 	game_fn_nothing_call_count++;
// }
// int32_t game_fn_magic() {
// 	game_fn_magic_call_count++;
// }
void game_fn_initialize(int32_t x) {
	game_fn_initialize_call_count++;

	game_fn_initialize_x = x;
}
// void game_fn_initialize_bool() {
// 	game_fn_initialize_bool_call_count++;
// }
// int32_t game_fn_identity() {
// 	game_fn_identity_call_count++;
// }
// int32_t game_fn_max() {
// 	game_fn_max_call_count++;
// }
// void game_fn_say() {
// 	game_fn_say_call_count++;
// }
// float game_fn_sin() {
// 	game_fn_sin_call_count++;
// }
// float game_fn_cos() {
// 	game_fn_cos_call_count++;
// }
// void game_fn_mega() {
// 	game_fn_mega_call_count++;
// }
// bool game_fn_is_friday() {
// 	game_fn_is_friday_call_count++;
// }
// void game_fn_set_is_happy() {
// 	game_fn_set_is_happy_call_count++;
// }
// void game_fn_mega_f32() {
// 	game_fn_mega_f32_call_count++;
// }
// void game_fn_mega_i32() {
// 	game_fn_mega_i32_call_count++;
// }

// void game_fn_define_a(void) {}
// void game_fn_define_b(int32_t x) {}
// void game_fn_define_c(int32_t x, int32_t y) {}
void game_fn_define_d(void) {}
// void game_fn_define_e(void) {}
// void game_fn_define_f(void) {}
// void game_fn_define_g(void) {}
// void game_fn_define_h(int32_t x) {}
// void game_fn_define_i(int32_t x, int32_t y) {}
// void game_fn_define_j(void) {}
// void game_fn_define_k(int32_t age, char *name) {}
// void game_fn_define_l(char *group, char *name) {}
// void game_fn_define_m(int32_t w, char *group, bool b1, char *name, bool b2, int32_t z) {}
// void game_fn_define_n(int32_t u, int32_t v, int32_t w, int32_t x, int32_t y, int32_t z) {}
// void game_fn_define_o(char *u, char *v, char *w, char *x, char *y, char *z) {}
// void game_fn_define_p(char *x) {}
// void game_fn_define_q(char *a, char *b, char *c) {}
// void game_fn_define_r(void) {}
// void game_fn_define_s(void) {}
// void game_fn_define_t(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8) {}

// void game_fn_define_k(int32_t age, char *name) {
// 	game_fn_define_k_age = age;
// 	game_fn_define_k_name = name;
// }

static void reset_call_counts(void) {
	game_fn_nothing_call_count = 0;
	game_fn_magic_call_count = 0;
	game_fn_initialize_call_count = 0;
	game_fn_initialize_bool_call_count = 0;
	game_fn_identity_call_count = 0;
	game_fn_max_call_count = 0;
	game_fn_say_call_count = 0;
	game_fn_sin_call_count = 0;
	game_fn_cos_call_count = 0;
	game_fn_mega_call_count = 0;
	game_fn_is_friday_call_count = 0;
	game_fn_set_is_happy_call_count = 0;
	game_fn_mega_f32_call_count = 0;
	game_fn_mega_i32_call_count = 0;
}

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

#define TEST_ERROR(test_name) test_error(\
	test_name,\
	"tests_err/"test_name"/input.grug",\
	"tests_err/"test_name"/expected_error.txt",\
	"tests_err/"test_name"/results",\
	"tests_err/"test_name"/results/output.so",\
	"tests_err/"test_name"/results/grug_output.txt"\
);

#define TEST_RUNTIME_ERROR(test_name) {\
	struct runtime_error_data data = runtime_error_prologue(\
		#test_name,\
		"tests_err_runtime/"#test_name"/input.grug",\
		"tests_err_runtime/"#test_name"/expected_error.txt",\
		"tests_err_runtime/"#test_name"/results",\
		"tests_err_runtime/"#test_name"/results/output.so",\
		"tests_err_runtime/"#test_name"/results/output.hex",\
		"tests_err_runtime/"#test_name"/results/output_elf.log",\
		"tests_err_runtime/"#test_name"/results/output_objdump.log"\
	);\
	if (data.run) {\
		runtime_error_##test_name(data.on_fns, data.g);\
		runtime_error_compare_against_expected_error("tests_err_runtime/"#test_name"/expected_error.txt");\
	}\
}

static size_t read_dll(char *dll_path, uint8_t *dll_bytes) {
	// TODO: Check if using mmap() makes this faster:
	// https://stackoverflow.com/a/174808/13279557
	FILE *f = fopen(dll_path, "r");
	check_null(f, "fopen");

	check(fseek(f, 0, SEEK_END), "fseek");

	long ftell_result = ftell(f);
	check(ftell_result, "ftell");
	size_t dll_bytes_len = ftell_result;

	check(fseek(f, 0, SEEK_SET), "fseek");

	if (fread(dll_bytes, dll_bytes_len, 1, f) == 0) {
		if (feof(f)) {
			printf("fread EOF\n");
		}
		if (ferror(f)) {
			printf("fread error\n");
		}
		exit(EXIT_FAILURE);
	}

    if (fclose(f) == EOF) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}

	return dll_bytes_len;
}

static void *get(void *handle, char *label) {
	void *p = dlsym(handle, label);
	if (!p) {
		printf("dlsym: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}
	return p;
}

static void run_and_write(char *const *argv, char *written_path) {
	pid_t pid = fork();
	check(pid, "fork");

	if (pid == 0) {
		int fd = open(written_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
		check(fd, "open");

		dup2(fd, STDOUT_FILENO); // Redirect stdout to written_path

		close(fd);

		check(execvp(argv[0], argv), "execvp");
	}

	// Wait on the child to finish
	check(wait(NULL), "wait");
}

static void run(char *const *argv) {
	pid_t pid = fork();
	check(pid, "fork");

	if (pid == 0) {
		check(execvp(argv[0], argv), "execvp");
	}

	// Wait on the child to finish
	check(wait(NULL), "wait");
}

static void output_dll_info(char *dll_path, char *xxd_path, char *readelf_path, char *objdump_path) {
	run((char *[]){"xxd", dll_path, xxd_path, NULL});

	// These don't work, for some reason, even when ">" is added to the start of the readelf_path/objdump_path
	// run((char *[]){"readelf", "-a", dll_path, ">", readelf_path, NULL});
	// run((char *[]){"readelf", "-a", dll_path, readelf_path, NULL});
	// run((char *[]){"objdump", "-D", dll_path, "-Mintel", ">", objdump_path, NULL});
	// run((char *[]){"objdump", "-D", dll_path, "-Mintel", objdump_path, NULL});

	run_and_write((char *[]){"readelf", "-a", dll_path, NULL}, readelf_path);

	run_and_write((char *[]){"objdump", "-D", dll_path, "-Mintel", NULL}, objdump_path);
}

static bool newer(char *path1, char *path2) {
	// printf("path1: %s\n", path1);
	// printf("path2: %s\n", path2);

	struct stat s1;
	if (stat(path1, &s1) == -1) {
		if (errno != ENOENT) {
			perror("stat");
			exit(EXIT_FAILURE);
		}
		// printf("path1 does not exist\n");
		return false;
	}

	struct stat s2;
	check(stat(path2, &s2), "stat");

	// printf("path1 mtime >= path2 mtime: %d\n", s1.st_mtime >= s2.st_mtime);

	// printf("path1 mtime: %ld\n", s1.st_mtime);
	// printf("path2 mtime: %ld\n", s2.st_mtime);

	return s1.st_mtime >= s2.st_mtime;
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

static void runtime_error_compare_against_expected_error(char *expected_error_path) {
	printf("  Comparing against the expected error...\n");

	char *grug_error_msg = grug_get_runtime_error_reason();

	size_t grug_error_msg_len = strlen(grug_error_msg);

	char *expected_error = get_expected_error(expected_error_path);
	size_t expected_error_len = strlen(expected_error);

	if (expected_error_len != grug_error_msg_len || memcmp(grug_error_msg, expected_error, expected_error_len) != 0) {
		printf("\nThe output differs from the expected output.\n");
		printf("Output:\n");
		printf("%s\n", grug_error_msg);

		printf("Expected:\n");
		printf("%s\n", expected_error);

		exit(EXIT_FAILURE);
	}
}

static void make_results_dir(char *results_path) {
	if (mkdir(results_path, 0755) == -1 && errno != EEXIST) {
		perror("mkdir");
		exit(EXIT_FAILURE);
	}
}

static int remove_callback(const char *entry_path, const struct stat *entry_info, int entry_type, struct FTW *ftw) {
	(void)entry_info;
	(void)entry_type;
	(void)ftw;

	int rv = remove(entry_path);

	check(rv, "remove");

	return rv;
}

static int rm_rf(char *path) {
	return nftw(path, remove_callback, 42, FTW_DEPTH | FTW_PHYS);
}

static void test_error(
	char *test_name,
	char *grug_path,
	char *expected_error_path,
	char *results_path,
	char *output_dll_path,
	char *grug_output_path
) {
	if (newer(grug_output_path, grug_path)
	 && newer(grug_output_path, expected_error_path)
	 && newer(grug_output_path, "mod_api.h")
	 && newer(grug_output_path, "mod_api.json")
	 && newer(grug_output_path, "tests.sh")
	 && newer(grug_output_path, "tests.out")
	) {
		printf("Skipping tests_err/%s...\n", test_name);
		return;
	}

	printf("Running tests_err/%s...\n", test_name);

	rm_rf(results_path);
	make_results_dir(results_path);

	printf("  Regenerating output.so...\n");

	assert(grug_test_regenerate_dll(grug_path, output_dll_path));

	printf("  Outputting grug_output.txt...\n");

	FILE *f = fopen(grug_output_path, "w");

	size_t grug_error_msg_len = strlen(grug_error.msg);

	if (fwrite(grug_error.msg, grug_error_msg_len, 1, f) == 0) {
		fprintf(stderr, "%s\n", "fwrite had an error\n");
		exit(EXIT_FAILURE);
	}

    if (fclose(f) == EOF) {
		perror("fclose");
		exit(EXIT_FAILURE);
		exit(EXIT_FAILURE);
	}

	printf("  Comparing against the expected error...\n");

	char *expected_error = get_expected_error(expected_error_path);
	size_t expected_error_len = strlen(expected_error);

	if (expected_error_len != grug_error_msg_len || memcmp(grug_error.msg, expected_error, expected_error_len) != 0) {
		printf("\nThe output differs from the expected output.\n");
		printf("Output:\n");
		printf("%s\n", grug_error.msg);

		printf("Expected:\n");
		printf("%s\n", expected_error);

		exit(EXIT_FAILURE);
	}
}

static struct runtime_error_data runtime_error_prologue(
	char *test_name,
	char *grug_path,
	char *expected_error_path,
	char *results_path,
	char *output_dll_path,
	char *output_xxd_path,
	char *output_readelf_path,
	char *output_objdump_path
) {
	if (newer(output_dll_path, grug_path)
	 && newer(output_dll_path, expected_error_path)
	 && newer(output_dll_path, "mod_api.h")
	 && newer(output_dll_path, "mod_api.json")
	 && newer(output_dll_path, "tests.sh")
	 && newer(output_dll_path, "tests.out")
	) {
		printf("Skipping tests_err_runtime/%s...\n", test_name);
		return (struct runtime_error_data){.run=false};
	}

	printf("Running tests_err_runtime/%s...\n", test_name);

	rm_rf(results_path);
	make_results_dir(results_path);

	printf("  Regenerating output.so...\n");

	if (grug_test_regenerate_dll(grug_path, output_dll_path)) {
		printf("The test wasn't supposed to print anything during generation of the dll, but did:\n");
		printf("----\n");
		printf("%s\n", grug_error.msg);
		printf("----\n");

		exit(EXIT_FAILURE);
	}

	printf("  Outputting output.so info...\n");
	output_dll_info(output_dll_path, output_xxd_path, output_readelf_path, output_objdump_path);

	printf("  Running the test...\n");

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
	#pragma GCC diagnostic pop

	void *on_fns = get(handle, "on_fns");

	return (struct runtime_error_data){.run=true, .on_fns=on_fns, .g=g};
}

static void runtime_error_division_by_0(void *on_fns, void *g) {
	bool had_runtime_error = false;

	if (grug_mod_had_runtime_error()) {
		had_runtime_error = true;
	}

	if (!had_runtime_error) {
		((struct d_on_fns *)on_fns)->a(g);
	}

	free(g);

	assert(had_runtime_error);
}

static bool handler_called;
static void handler(int sig) {
	(void)sig;
	handler_called = true;
}

static void runtime_error_raise_alrm_with_handler(void *on_fns, void *g) {
	bool had_runtime_error = false;

	if (grug_mod_had_runtime_error()) {
		had_runtime_error = true;

		handler_called = false;
		raise(SIGALRM);
		assert(handler_called);
	}

	signal(SIGALRM, handler);

	if (!had_runtime_error) {
		((struct d_on_fns *)on_fns)->a(g);
	}

	free(g);

	assert(had_runtime_error);
}

static void ok_addition_as_argument(void) {
	char *grug_path = "tests_ok/addition_as_argument/input.grug";
	char *nasm_path = "tests_ok/addition_as_argument/input.s";
	char *results_path = "tests_ok/addition_as_argument/results";
	char *output_dll_path = "tests_ok/addition_as_argument/results/output.so";
	char *expected_dll_path = "tests_ok/addition_as_argument/results/expected.so";
	char *nasm_o_path = "tests_ok/addition_as_argument/results/expected.o";
	char *output_xxd_path = "tests_ok/addition_as_argument/results/output.hex";
	char *output_readelf_path = "tests_ok/addition_as_argument/results/output_elf.log";
	char *output_objdump_path = "tests_ok/addition_as_argument/results/output_objdump.log";
	char *expected_xxd_path = "tests_ok/addition_as_argument/results/expected.hex";
	char *expected_readelf_path = "tests_ok/addition_as_argument/results/expected_elf.log";
	char *expected_objdump_path = "tests_ok/addition_as_argument/results/expected_objdump.log";

	if (newer(output_dll_path, nasm_path)
	 && newer(output_dll_path, grug_path)
	 && newer(output_dll_path, expected_dll_path)
	 && newer(output_dll_path, "mod_api.h")
	 && newer(output_dll_path, "mod_api.json")
	 && newer(output_dll_path, "tests.sh")
	 && newer(output_dll_path, "tests.out")
	) {
		printf("Skipping ok_addition_as_argument...\n");
		return;
	}

	printf("Running ok_addition_as_argument...\n");

	reset_call_counts();

	rm_rf(results_path);
	make_results_dir(results_path);

	struct stat nasm_stat;
	check(stat(nasm_path, &nasm_stat), "stat");

	printf("  Regenerating expected.so...\n");

	run((char *[]){"nasm", nasm_path, "-felf64", "-O0", "-o", nasm_o_path, NULL});

	run((char *[]){"ld", nasm_o_path, "-o", expected_dll_path, "-shared", "--hash-style=sysv", NULL});

	remove(nasm_o_path);

	static char redefine_sym[420];

	size_t nasm_path_len = strlen(nasm_path);

	strcpy(redefine_sym, nasm_path);
	redefine_sym[nasm_path_len] = '=';
	strcpy(redefine_sym + nasm_path_len + 1, grug_path);

	run((char *[]){"objcopy", expected_dll_path, "--redefine-sym", redefine_sym, NULL});

	printf("  Outputting expected.so info...\n");
	output_dll_info(expected_dll_path, expected_xxd_path, expected_readelf_path, expected_objdump_path);

	printf("  Running the test...\n");

	void *handle = dlopen(expected_dll_path, RTLD_NOW);
	if (!handle) {
		fprintf(stderr, "dlopen: %s\n", dlerror());
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

	assert(game_fn_initialize_call_count == 0);
	on_fns->a(g);
	assert(game_fn_initialize_call_count == 1);

	assert(game_fn_initialize_x == 3);

	assert(strcmp(grug_on_fn_name, "on_a") == 0);
	assert(strcmp(grug_on_fn_path, "tests_ok/addition_as_argument/input.grug") == 0);

	free(g);

	printf("  Regenerating output.so...\n");

	if (grug_test_regenerate_dll(grug_path, output_dll_path)) {
		printf("The test wasn't supposed to print anything, but did:\n");
		printf("----\n");
		printf("%s\n", grug_error.msg);
		printf("----\n");

		exit(EXIT_FAILURE);
	}

	printf("  Outputting output.so info...\n");
	output_dll_info(output_dll_path, output_xxd_path, output_readelf_path, output_objdump_path);

	printf("  Comparing output.so against expected.so...\n");

	static uint8_t output_dll_bytes[420420];
	size_t output_dll_bytes_len = read_dll(output_dll_path, output_dll_bytes);

	static uint8_t expected_dll_bytes[420420];
	size_t expected_dll_bytes_len = read_dll(expected_dll_path, expected_dll_bytes);

	if (expected_dll_bytes_len != output_dll_bytes_len || memcmp(output_dll_bytes, expected_dll_bytes, expected_dll_bytes_len) != 0) {
		printf("\nThe output differs from the expected output.\n");
		exit(EXIT_FAILURE);
	}
}

static void error_tests(void) {
	TEST_ERROR("assign_to_unknown_variable");
	// TEST_ERROR("assignment_isnt_expression");
	// TEST_ERROR("assign_to_unknown_variable");
	// TEST_ERROR("assignment_isnt_expression");
	// TEST_ERROR("bool_unary_minus");
	// TEST_ERROR("cant_add_strings");
	// TEST_ERROR("cant_call_define_fn_1");
	// TEST_ERROR("cant_call_define_fn_2");
	// TEST_ERROR("cant_redefine_global");
	// TEST_ERROR("define_fn_calls_fn");
	// TEST_ERROR("define_fn_different_name");
	// TEST_ERROR("define_fn_not_enough_arguments");
	// TEST_ERROR("define_fn_only_one_max");
	// TEST_ERROR("define_fn_uses_global_variable");
	// TEST_ERROR("define_fn_was_not_declared");
	// TEST_ERROR("f32_missing_digit_after_decimal_point");
	// TEST_ERROR("game_fn_does_not_exist");
	// TEST_ERROR("game_function_call_gets_wrong_arg_type");
	// TEST_ERROR("game_function_call_less_args_expected");
	// TEST_ERROR("game_function_call_more_args_expected");
	// TEST_ERROR("game_function_call_no_args_expected");
	// TEST_ERROR("global_variable_already_uses_local_variable_name");
	// TEST_ERROR("global_variable_before_define");
	// TEST_ERROR("global_variable_calls_fn");
	// TEST_ERROR("global_variable_definition_cant_use_itself");
	// TEST_ERROR("global_variable_definition_requires_value_i32");
	// TEST_ERROR("global_variable_definition_requires_value_string");
	// TEST_ERROR("global_variable_uses_global_variable");
	// TEST_ERROR("helper_fn_does_not_exist");
	// TEST_ERROR("helper_function_call_gets_wrong_arg_type");
	// TEST_ERROR("helper_function_call_less_args_expected");
	// TEST_ERROR("helper_function_call_more_args_expected");
	// TEST_ERROR("helper_function_call_no_args_expected");
	// TEST_ERROR("helper_function_different_return_value_expected");
	// TEST_ERROR("helper_function_missing_return_statement");
	// TEST_ERROR("helper_function_no_return_value_expected");
	// TEST_ERROR("i32_logical_not");
	// TEST_ERROR("i32_too_big");
	// TEST_ERROR("i32_too_small");
	// TEST_ERROR("local_variable_already_exists");
	// TEST_ERROR("local_variable_definition_cant_use_itself");
	// TEST_ERROR("missing_define_fn");
	// TEST_ERROR("no_space_between_comment_character_and_comment");
	// TEST_ERROR("on_fn_before_define");
	// TEST_ERROR("on_fn_duplicate");
	// TEST_ERROR("on_fn_was_not_declared_in_entity");
	// TEST_ERROR("on_fn_wrong_order");
	// TEST_ERROR("on_function_gets_wrong_arg_type");
	// TEST_ERROR("on_function_less_args_expected");
	// TEST_ERROR("on_function_more_args_expected");
	// TEST_ERROR("on_function_no_args_expected");
	// TEST_ERROR("on_function_no_return_value_expected");
	// TEST_ERROR("pass_bool_to_i32_game_param");
	// TEST_ERROR("pass_bool_to_i32_helper_param");
	// TEST_ERROR("resource_type_for_global");
	// TEST_ERROR("resource_type_for_helper_fn_argument");
	// TEST_ERROR("resource_type_for_helper_fn_return_type");
	// TEST_ERROR("resource_type_for_local");
	// TEST_ERROR("resource_type_for_on_fn_argument");
	// TEST_ERROR("string_pointer_arithmetic");
	// TEST_ERROR("too_many_f32_arguments");
	// TEST_ERROR("too_many_i32_arguments");
	// TEST_ERROR("trailing_space_in_comment");
	// TEST_ERROR("unclosed_double_quote");
	// TEST_ERROR("unknown_variable");
	// TEST_ERROR("unused_result");
	// TEST_ERROR("variable_assignment_before_definition");
	// TEST_ERROR("variable_definition_requires_value_i32");
	// TEST_ERROR("variable_definition_requires_value_string");
	// TEST_ERROR("variable_statement_missing_assignment");
	// TEST_ERROR("variable_used_before_definition");
	// TEST_ERROR("wrong_type_global_assignment");
	// TEST_ERROR("wrong_type_global_reassignment");
	// TEST_ERROR("wrong_type_local_assignment");
	// TEST_ERROR("wrong_type_local_reassignment");
}

static void runtime_error_tests(void) {
	TEST_RUNTIME_ERROR(division_by_0);
	TEST_RUNTIME_ERROR(raise_alrm_with_handler);
	// TEST_RUNTIME_ERROR(raise_fpe_with_handler);
	// TEST_RUNTIME_ERROR(raise_sigsegv_with_handler);
	// TEST_RUNTIME_ERROR(stack_overflow);
	// TEST_RUNTIME_ERROR(time_limit_exceeded);
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

	printf("\nAll tests passed!\n");
}
