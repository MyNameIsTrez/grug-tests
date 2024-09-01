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

struct test_data {
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
static size_t game_fn_blocked_alrm_call_count;
static size_t game_fn_nothing_aligned_call_count;
static size_t game_fn_magic_aligned_call_count;
static size_t game_fn_initialize_aligned_call_count;

static bool streq(char *a, char *b) {
	return strcmp(a, b) == 0;
}

void game_fn_nothing(void) {
	game_fn_nothing_call_count++;
}
int32_t game_fn_magic(void) {
	game_fn_magic_call_count++;

	return 42;
}
static int32_t game_fn_initialize_x;
void game_fn_initialize(int32_t x) {
	game_fn_initialize_call_count++;

	game_fn_initialize_x = x;
}
static bool game_fn_initialize_bool_b;
void game_fn_initialize_bool(bool b) {
	game_fn_initialize_bool_call_count++;

	game_fn_initialize_bool_b = b;
}
static int32_t game_fn_identity_x;
int32_t game_fn_identity(int32_t x) {
	game_fn_identity_call_count++;

	game_fn_identity_x = x;

	return x;
}
static int32_t game_fn_max_x;
static int32_t game_fn_max_y;
int32_t game_fn_max(int32_t x, int32_t y) {
	game_fn_max_call_count++;

	game_fn_max_x = x;
	game_fn_max_y = y;

	return x > y ? x : y;
}
static char *game_fn_say_message;
void game_fn_say(char *message) {
	game_fn_say_call_count++;

	game_fn_say_message = message;
}
static float game_fn_sin_x;
float game_fn_sin(float x) {
	game_fn_sin_call_count++;

	game_fn_sin_x = x;

	return sinf(x);
}
static float game_fn_cos_x;
float game_fn_cos(float x) {
	game_fn_cos_call_count++;

	game_fn_cos_x = x;

	return cosf(x);
}
static float game_fn_mega_f1;
static int32_t game_fn_mega_i1;
static bool game_fn_mega_b1;
static float game_fn_mega_f2;
static float game_fn_mega_f3;
static float game_fn_mega_f4;
static bool game_fn_mega_b2;
static int32_t game_fn_mega_i2;
static float game_fn_mega_f5;
static float game_fn_mega_f6;
static float game_fn_mega_f7;
static float game_fn_mega_f8;
static int32_t game_fn_mega_i3;
static char *game_fn_mega_str;
void game_fn_mega(float f1, int32_t i1, bool b1, float f2, float f3, float f4, bool b2, int32_t i2, float f5, float f6, float f7, float f8, int32_t i3, char *str) {
	game_fn_mega_call_count++;

	game_fn_mega_f1 = f1;
	game_fn_mega_i1 = i1;
	game_fn_mega_b1 = b1;
	game_fn_mega_f2 = f2;
	game_fn_mega_f3 = f3;
	game_fn_mega_f4 = f4;
	game_fn_mega_b2 = b2;
	game_fn_mega_i2 = i2;
	game_fn_mega_f5 = f5;
	game_fn_mega_f6 = f6;
	game_fn_mega_f7 = f7;
	game_fn_mega_f8 = f8;
	game_fn_mega_i3 = i3;
	game_fn_mega_str = str;
}
bool game_fn_is_friday(void) {
	game_fn_is_friday_call_count++;

	return true;
}
static bool game_fn_set_is_happy_is_happy;
void game_fn_set_is_happy(bool is_happy) {
	game_fn_set_is_happy_call_count++;

	game_fn_set_is_happy_is_happy = is_happy;
}
static float game_fn_mega_f32_f1;
static float game_fn_mega_f32_f2;
static float game_fn_mega_f32_f3;
static float game_fn_mega_f32_f4;
static float game_fn_mega_f32_f5;
static float game_fn_mega_f32_f6;
static float game_fn_mega_f32_f7;
static float game_fn_mega_f32_f8;
static float game_fn_mega_f32_f9;
void game_fn_mega_f32(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9) {
	game_fn_mega_f32_call_count++;

	game_fn_mega_f32_f1 = f1;
	game_fn_mega_f32_f2 = f2;
	game_fn_mega_f32_f3 = f3;
	game_fn_mega_f32_f4 = f4;
	game_fn_mega_f32_f5 = f5;
	game_fn_mega_f32_f6 = f6;
	game_fn_mega_f32_f7 = f7;
	game_fn_mega_f32_f8 = f8;
	game_fn_mega_f32_f9 = f9;
}
static int32_t game_fn_mega_i32_i1;
static int32_t game_fn_mega_i32_i2;
static int32_t game_fn_mega_i32_i3;
static int32_t game_fn_mega_i32_i4;
static int32_t game_fn_mega_i32_i5;
static int32_t game_fn_mega_i32_i6;
static int32_t game_fn_mega_i32_i7;
void game_fn_mega_i32(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7) {
	game_fn_mega_i32_call_count++;

	game_fn_mega_i32_i1 = i1;
	game_fn_mega_i32_i2 = i2;
	game_fn_mega_i32_i3 = i3;
	game_fn_mega_i32_i4 = i4;
	game_fn_mega_i32_i5 = i5;
	game_fn_mega_i32_i6 = i6;
	game_fn_mega_i32_i7 = i7;
}
void game_fn_blocked_alrm(void) {
	game_fn_blocked_alrm_call_count++;

	sigset_t mask;

	sigprocmask(SIG_BLOCK, NULL, &mask);

	assert(sigismember(&mask, SIGALRM));
}
void game_fn_nothing_aligned(void) {
	game_fn_nothing_aligned_call_count++;

	// From https://stackoverflow.com/a/2114249/13279557
	int64_t rsp;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wlanguage-extension-token"
	asm ("mov %%rsp, %0" : "=r" (rsp) );
	#pragma GCC diagnostic pop

	// We need this in order to ensure that the C compiler will 16-byte align
	// this function with a function prologue, cause we assert that the rsp is divisible by 16 after the function prologue
	printf(":)\n");

	assert((rsp & 0xf) == 0);
}
int32_t game_fn_magic_aligned(void) {
	game_fn_magic_aligned_call_count++;

	// From https://stackoverflow.com/a/2114249/13279557
	int64_t rsp;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wlanguage-extension-token"
	asm ("mov %%rsp, %0" : "=r" (rsp) );
	#pragma GCC diagnostic pop

	printf(":)\n");

	assert((rsp & 0xf) == 0);

	return 42;
}
static int32_t game_fn_initialize_aligned_x;
void game_fn_initialize_aligned(int32_t x) {
	game_fn_initialize_aligned_call_count++;

	game_fn_initialize_aligned_x = x;

	// From https://stackoverflow.com/a/2114249/13279557
	int64_t rsp;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wlanguage-extension-token"
	asm ("mov %%rsp, %0" : "=r" (rsp) );
	#pragma GCC diagnostic pop

	// We need this in order to ensure that the C compiler will 16-byte align
	// this function with a function prologue, cause we assert that the rsp is divisible by 16 after the function prologue
	printf(":)\n");

	assert((rsp & 0xf) == 0);
}

void game_fn_define_a(void) {}
static int32_t game_fn_define_b_x;
void game_fn_define_b(int32_t x) {
	game_fn_define_b_x = x;
}
static int32_t game_fn_define_c_x;
static int32_t game_fn_define_c_y;
void game_fn_define_c(int32_t x, int32_t y) {
	game_fn_define_c_x = x;
	game_fn_define_c_y = y;
}
void game_fn_define_d(void) {}
void game_fn_define_e(void) {}
void game_fn_define_f(void) {}
void game_fn_define_g(void) {}
static int32_t game_fn_define_h_x;
void game_fn_define_h(int32_t x) {
	game_fn_define_h_x = x;
}
static int32_t game_fn_define_i_x;
static int32_t game_fn_define_i_y;
void game_fn_define_i(int32_t x, int32_t y) {
	game_fn_define_i_x = x;
	game_fn_define_i_y = y;
}
void game_fn_define_j(void) {}
static int32_t game_fn_define_k_age;
static char *game_fn_define_k_name;
void game_fn_define_k(int32_t age, char *name) {
	game_fn_define_k_age = age;
	game_fn_define_k_name = name;
}
static char *game_fn_define_l_group;
static char *game_fn_define_l_name;
void game_fn_define_l(char *group, char *name) {
	game_fn_define_l_group = group;
	game_fn_define_l_name = name;
}
static int32_t game_fn_define_m_w;
static char *game_fn_define_m_group;
static bool game_fn_define_m_b1;
static char *game_fn_define_m_name;
static bool game_fn_define_m_b2;
static int32_t game_fn_define_m_z;
void game_fn_define_m(int32_t w, char *group, bool b1, char *name, bool b2, int32_t z) {
	game_fn_define_m_w = w;
	game_fn_define_m_group = group;
	game_fn_define_m_b1 = b1;
	game_fn_define_m_name = name;
	game_fn_define_m_b2 = b2;
	game_fn_define_m_z = z;
}
static int32_t game_fn_define_n_u;
static int32_t game_fn_define_n_v;
static int32_t game_fn_define_n_w;
static int32_t game_fn_define_n_x;
static int32_t game_fn_define_n_y;
static int32_t game_fn_define_n_z;
void game_fn_define_n(int32_t u, int32_t v, int32_t w, int32_t x, int32_t y, int32_t z) {
	game_fn_define_n_u = u;
	game_fn_define_n_v = v;
	game_fn_define_n_w = w;
	game_fn_define_n_x = x;
	game_fn_define_n_y = y;
	game_fn_define_n_z = z;
}
static char *game_fn_define_o_u;
static char *game_fn_define_o_v;
static char *game_fn_define_o_w;
static char *game_fn_define_o_x;
static char *game_fn_define_o_y;
static char *game_fn_define_o_z;
void game_fn_define_o(char *u, char *v, char *w, char *x, char *y, char *z) {
	game_fn_define_o_u = u;
	game_fn_define_o_v = v;
	game_fn_define_o_w = w;
	game_fn_define_o_x = x;
	game_fn_define_o_y = y;
	game_fn_define_o_z = z;
}
static char *game_fn_define_p_x;
void game_fn_define_p(char *x) {
	game_fn_define_p_x = x;
}
static char *game_fn_define_q_a;
static char *game_fn_define_q_b;
static char *game_fn_define_q_c;
void game_fn_define_q(char *a, char *b, char *c) {
	game_fn_define_q_a = a;
	game_fn_define_q_b = b;
	game_fn_define_q_c = c;
}
void game_fn_define_r(void) {}
void game_fn_define_s(void) {}
static float game_fn_define_t_f1;
static float game_fn_define_t_f2;
static float game_fn_define_t_f3;
static float game_fn_define_t_f4;
static float game_fn_define_t_f5;
static float game_fn_define_t_f6;
static float game_fn_define_t_f7;
static float game_fn_define_t_f8;
void game_fn_define_t(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8) {
	game_fn_define_t_f1 = f1;
	game_fn_define_t_f2 = f2;
	game_fn_define_t_f3 = f3;
	game_fn_define_t_f4 = f4;
	game_fn_define_t_f5 = f5;
	game_fn_define_t_f6 = f6;
	game_fn_define_t_f7 = f7;
	game_fn_define_t_f8 = f8;
}
static char *game_fn_define_u_sprite_path;
void game_fn_define_u(char *sprite_path) {
	game_fn_define_u_sprite_path = sprite_path;
}

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
	game_fn_blocked_alrm_call_count = 0;
	game_fn_nothing_aligned_call_count = 0;
	game_fn_magic_aligned_call_count = 0;
	game_fn_initialize_aligned_call_count = 0;
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
	#test_name,\
	"tests/err/"#test_name"/input.grug",\
	"tests/err/"#test_name"/expected_error.txt",\
	"tests/err/"#test_name"/results",\
	"tests/err/"#test_name"/results/output.so",\
	"tests/err/"#test_name"/results/grug_output.txt",\
	"tests/err/"#test_name"/results/failed"\
);

#define TEST_RUNTIME_ERROR(test_name) {\
	struct test_data data = runtime_error_prologue(\
		#test_name,\
		"tests/err_runtime/"#test_name"/input.grug",\
		"tests/err_runtime/"#test_name"/expected_error.txt",\
		"tests/err_runtime/"#test_name"/results",\
		"tests/err_runtime/"#test_name"/results/output.so",\
		"tests/err_runtime/"#test_name"/results/output.hex",\
		"tests/err_runtime/"#test_name"/results/output_elf.log",\
		"tests/err_runtime/"#test_name"/results/output_objdump.log",\
		"tests/err_runtime/"#test_name"/results/failed"\
	);\
	if (data.run) {\
		runtime_error_##test_name(data.on_fns, data.g);\
		runtime_error_epilogue(\
			"tests/err_runtime/"#test_name"/expected_error.txt",\
			"tests/err_runtime/"#test_name"/results/failed"\
		);\
	}\
}

#define TEST_OK(test_name, expected_define_type, expected_globals_size) {\
	struct test_data data = ok_prologue(\
		#test_name,\
		"tests/ok/"#test_name"/input.grug",\
		"tests/ok/"#test_name"/input.s",\
		"tests/ok/"#test_name"/results",\
		"tests/ok/"#test_name"/results/output.so",\
		"tests/ok/"#test_name"/results/expected.so",\
		"tests/ok/"#test_name"/results/expected.o",\
		"tests/ok/"#test_name"/results/expected.hex",\
		"tests/ok/"#test_name"/results/expected_elf.log",\
		"tests/ok/"#test_name"/results/expected_objdump.log",\
		"tests/ok/"#test_name"/results/failed",\
		expected_define_type,\
		expected_globals_size\
	);\
	if (data.run) {\
		ok_##test_name(data.on_fns, data.g);\
		ok_epilogue(\
			"tests/ok/"#test_name"/input.grug",\
			"tests/ok/"#test_name"/results/output.so",\
			"tests/ok/"#test_name"/results/expected.so",\
			"tests/ok/"#test_name"/results/output.hex",\
			"tests/ok/"#test_name"/results/output_elf.log",\
			"tests/ok/"#test_name"/results/output_objdump.log",\
			"tests/ok/"#test_name"/results/failed"\
		);\
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
	pid_t pid = fork();
	check(pid, "fork");

	if (pid == 0) {
		// In newer versions off xxd -Rnever turns colorization off,
		// but in older versions of xxd the -R option didn't exist yet,
		// so we can't use that
		int fd = open(xxd_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		check(fd, "open");

		check(dup2(fd, STDOUT_FILENO), "dup2");

		check(close(fd), "close");

		check(execvp("xxd", (char *[]){"xxd", dll_path, NULL}), "execvp");
	}

	// Wait on the child to finish
	check(wait(NULL), "wait");

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

static void create_failed_file(char *failed_file_path) {
	int fd = open(failed_file_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
	check(fd, "open");
	close(fd);
}

static bool failed_file_doesnt_exist(char *failed_file_path) {
	errno = 0;
	return access(failed_file_path, F_OK) == -1 && errno == ENOENT;
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
	char *grug_output_path,
	char *failed_file_path
) {
	if (failed_file_doesnt_exist(failed_file_path)
	 && newer(grug_output_path, grug_path)
	 && newer(grug_output_path, expected_error_path)
	 && newer(grug_output_path, "mod_api.h")
	 && newer(grug_output_path, "mod_api.json")
	 && newer(grug_output_path, "tests.sh")
	 && newer(grug_output_path, "tests.out")
	) {
		printf("Skipping tests/err/%s...\n", test_name);
		return;
	}

	printf("Running tests/err/%s...\n", test_name);

	rm_rf(results_path);
	make_results_dir(results_path);

	create_failed_file(failed_file_path);

	// printf("  Regenerating output.so...\n");

	assert(grug_test_regenerate_dll(grug_path, output_dll_path, "err"));

	// printf("  Outputting grug_output.txt...\n");

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

	// printf("  Comparing against the expected error...\n");

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

	unlink(failed_file_path);
}

static void runtime_error_epilogue(char *expected_error_path, char *failed_file_path) {
	// printf("  Comparing against the expected error...\n");

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

	unlink(failed_file_path);
}

static struct test_data runtime_error_prologue(
	char *test_name,
	char *grug_path,
	char *expected_error_path,
	char *results_path,
	char *output_dll_path,
	char *output_xxd_path,
	char *output_readelf_path,
	char *output_objdump_path,
	char *failed_file_path
) {
	if (failed_file_doesnt_exist(failed_file_path)
	 && newer(output_dll_path, grug_path)
	 && newer(output_dll_path, expected_error_path)
	 && newer(output_dll_path, "mod_api.h")
	 && newer(output_dll_path, "mod_api.json")
	 && newer(output_dll_path, "tests.sh")
	 && newer(output_dll_path, "tests.out")
	) {
		printf("Skipping tests/err_runtime/%s...\n", test_name);
		return (struct test_data){.run=false};
	}

	printf("Running tests/err_runtime/%s...\n", test_name);

	rm_rf(results_path);
	make_results_dir(results_path);

	create_failed_file(failed_file_path);

	// printf("  Regenerating output.so...\n");

	if (grug_test_regenerate_dll(grug_path, output_dll_path, "err_runtime")) {
		printf("The test wasn't supposed to print anything during generation of the dll, but did:\n");
		printf("----\n");
		printf("%s\n", grug_error.msg);
		printf("----\n");

		exit(EXIT_FAILURE);
	}

	// printf("  Outputting output.so info...\n");
	output_dll_info(output_dll_path, output_xxd_path, output_readelf_path, output_objdump_path);

	// printf("  Running the test...\n");

	void *handle = dlopen(output_dll_path, RTLD_NOW);
	if (!handle) {
		printf("dlopen: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	assert(streq(get(handle, "define_type"), "d"));

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

	return (struct test_data){.run=true, .on_fns=on_fns, .g=g};
}

static bool handler_called;
static void handler(int sig) {
	(void)sig;
	handler_called = true;
}

static void runtime_error_time_limit_exceeded(void *on_fns, void *g) {
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

static void runtime_error_division_by_0(void *on_fns, void *g) {
	bool had_runtime_error = false;

	if (grug_mod_had_runtime_error()) {
		had_runtime_error = true;

		handler_called = false;
		raise(SIGFPE);
		assert(handler_called);
	}

	signal(SIGFPE, handler);

	if (!had_runtime_error) {
		((struct d_on_fns *)on_fns)->a(g);
	}

	free(g);

	assert(had_runtime_error);
}

static void runtime_error_stack_overflow(void *on_fns, void *g) {
	bool had_runtime_error = false;

	if (grug_mod_had_runtime_error()) {
		had_runtime_error = true;

		handler_called = false;
		raise(SIGSEGV);
		assert(handler_called);
	}

	signal(SIGSEGV, handler);

	if (!had_runtime_error) {
		((struct d_on_fns *)on_fns)->a(g);
	}

	free(g);

	assert(had_runtime_error);
}

static void ok_epilogue(
	char *grug_path,
	char *output_dll_path,
	char *expected_dll_path,
	char *output_xxd_path,
	char *output_readelf_path,
	char *output_objdump_path,
	char *failed_file_path
) {
	// printf("  Regenerating output.so...\n");

	if (grug_test_regenerate_dll(grug_path, output_dll_path, "ok")) {
		printf("The test wasn't supposed to print anything, but did:\n");
		printf("----\n");
		printf("%s\n", grug_error.msg);
		printf("----\n");

		exit(EXIT_FAILURE);
	}

	// printf("  Outputting output.so info...\n");
	output_dll_info(output_dll_path, output_xxd_path, output_readelf_path, output_objdump_path);

	// printf("  Comparing output.so against expected.so...\n");

	static uint8_t output_dll_bytes[420420];
	size_t output_dll_bytes_len = read_dll(output_dll_path, output_dll_bytes);

	static uint8_t expected_dll_bytes[420420];
	size_t expected_dll_bytes_len = read_dll(expected_dll_path, expected_dll_bytes);

	if (expected_dll_bytes_len != output_dll_bytes_len || memcmp(output_dll_bytes, expected_dll_bytes, expected_dll_bytes_len) != 0) {
		printf("\nThe output differs from the expected output.\n");
		exit(EXIT_FAILURE);
	}

	unlink(failed_file_path);
}

static struct test_data ok_prologue(
	char *test_name,
	char *grug_path,
	char *nasm_path,
	char *results_path,
	char *output_dll_path,
	char *expected_dll_path,
	char *nasm_o_path,
	char *expected_xxd_path,
	char *expected_readelf_path,
	char *expected_objdump_path,
	char *failed_file_path,
	char *expected_define_type,
	size_t expected_globals_size
) {
	if (failed_file_doesnt_exist(failed_file_path)
	 && newer(output_dll_path, nasm_path)
	 && newer(output_dll_path, grug_path)
	 && newer(output_dll_path, expected_dll_path)
	 && newer(output_dll_path, "mod_api.h")
	 && newer(output_dll_path, "mod_api.json")
	 && newer(output_dll_path, "tests.sh")
	 && newer(output_dll_path, "tests.out")
	) {
		printf("Skipping tests/ok/%s...\n", test_name);
		return (struct test_data){.run=false};
	}

	printf("Running tests/ok/%s...\n", test_name);

	reset_call_counts();

	rm_rf(results_path);
	make_results_dir(results_path);

	create_failed_file(failed_file_path);

	struct stat nasm_stat;
	check(stat(nasm_path, &nasm_stat), "stat");

	// printf("  Regenerating expected.so...\n");

	run((char *[]){"nasm", nasm_path, "-felf64", "-O0", "-o", nasm_o_path, NULL});

	run((char *[]){"ld", nasm_o_path, "-o", expected_dll_path, "-shared", "--hash-style=sysv", NULL});

	static char redefine_sym[420];

	size_t nasm_path_len = strlen(nasm_path);

	strcpy(redefine_sym, nasm_path);
	redefine_sym[nasm_path_len] = '=';
	strcpy(redefine_sym + nasm_path_len + 1, grug_path);

	run((char *[]){"objcopy", expected_dll_path, "--redefine-sym", redefine_sym, NULL});

	// printf("  Outputting expected.so info...\n");
	output_dll_info(expected_dll_path, expected_xxd_path, expected_readelf_path, expected_objdump_path);

	// printf("  Running the test...\n");

	void *handle = dlopen(expected_dll_path, RTLD_NOW);
	if (!handle) {
		fprintf(stderr, "dlopen: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	assert(streq(get(handle, "define_type"), expected_define_type));

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	grug_define_fn_t define = get(handle, "define");
	define();

	size_t globals_size = *(size_t *)get(handle, "globals_size");
	assert(globals_size == expected_globals_size);

	void *g = malloc(globals_size);
	grug_init_globals_fn_t init_globals = get(handle, "init_globals");
	init_globals(g);
	#pragma GCC diagnostic pop

	void *on_fns = dlsym(handle, "on_fns");

	return (struct test_data){.run=true, .on_fns=on_fns, .g=g};
}

static void ok_addition_as_argument(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 3);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/addition_as_argument/input.grug"));
}

static void ok_addition_as_two_arguments(void *on_fns, void *g) {
	assert(game_fn_max_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_max_call_count == 1);

	free(g);

	assert(game_fn_max_x == 3);
	assert(game_fn_max_y == 9);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_addition_i32_wraparound(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == INT32_MIN);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_addition_with_multiplication(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 14);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_addition_with_multiplication_2(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 10);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_and_false_1(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_and_false_2(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_and_false_3(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_and_short_circuit(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_and_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_blocked_alrm(void *on_fns, void *g) {
	assert(game_fn_blocked_alrm_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_blocked_alrm_call_count == 1);

	free(g);

	// assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_bool_logical_not_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_bool_logical_not_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_bool_returned(void *on_fns, void *g) {
	assert(game_fn_set_is_happy_call_count == 0);
	assert(game_fn_is_friday_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_is_happy_call_count == 1);
	assert(game_fn_is_friday_call_count == 1);

	free(g);

	assert(game_fn_set_is_happy_is_happy == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_break(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 3);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_calls_100(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 100);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_calls_1000(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1000);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_continue(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_define_containing_addition(void *on_fns, void *g) {
	(void)on_fns;

	assert(game_fn_define_b_x == 3);

	free(g);
}

static void ok_define_containing_string(void *on_fns, void *g) {
	(void)on_fns;

	assert(game_fn_define_k_age == 42);
	assert(streq(game_fn_define_k_name, "foo"));

	free(g);
}

static void ok_define_with_eight_f32_fields(void *on_fns, void *g) {
	(void)on_fns;

	assert(game_fn_define_t_f1 == 1.0f);
	assert(game_fn_define_t_f2 == 2.0f);
	assert(game_fn_define_t_f3 == 3.0f);
	assert(game_fn_define_t_f4 == 4.0f);
	assert(game_fn_define_t_f5 == 5.0f);
	assert(game_fn_define_t_f6 == 6.0f);
	assert(game_fn_define_t_f7 == 7.0f);
	assert(game_fn_define_t_f8 == 8.0f);

	free(g);
}

static void ok_define_with_six_fields(void *on_fns, void *g) {
	(void)on_fns;

	assert(game_fn_define_m_w == 42);
	assert(streq(game_fn_define_m_group, "bar"));
	assert(game_fn_define_m_b1 == true);
	assert(streq(game_fn_define_m_name, "foobar"));
	assert(game_fn_define_m_b2 == false);
	assert(game_fn_define_m_z == 1337);

	free(g);
}

static void ok_define_with_six_i32_fields(void *on_fns, void *g) {
	(void)on_fns;

	assert(game_fn_define_n_u == 1);
	assert(game_fn_define_n_v == 2);
	assert(game_fn_define_n_w == 3);
	assert(game_fn_define_n_x == 4);
	assert(game_fn_define_n_y == 5);
	assert(game_fn_define_n_z == 6);

	free(g);
}

static void ok_define_with_six_string_fields(void *on_fns, void *g) {
	(void)on_fns;

	assert(streq(game_fn_define_o_u, "u"));
	assert(streq(game_fn_define_o_v, "v"));
	assert(streq(game_fn_define_o_w, "w"));
	assert(streq(game_fn_define_o_x, "x"));
	assert(streq(game_fn_define_o_y, "y"));
	assert(streq(game_fn_define_o_z, "z"));

	free(g);
}

static void ok_division_negative_result(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -2);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_division_positive_result(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 2);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_else_false(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_else_true(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 3);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_eq_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_eq_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_addition(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 6.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_argument(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 4.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_division(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 0.5f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_eq_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_eq_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_ge_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_ge_true_1(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_ge_true_2(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_global_variable(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 4.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_gt_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_gt_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_le_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_le_true_1(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_le_true_2(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_local_variable(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 4.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_lt_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_lt_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_multiplication(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 8.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_ne_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_negated(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == -4.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_ne_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_passed_to_helper_fn(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 42.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_passed_to_on_fn(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	((struct r_on_fns *)on_fns)->a(g, 42.0f);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 42.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_passing_sin_to_cos(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	assert(game_fn_cos_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);
	assert(game_fn_cos_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 4.0f);
	assert(game_fn_cos_x == sinf(4.0f));

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_f32_subtraction(void *on_fns, void *g) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == -2.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_fibonacci(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 55);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_ge_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_ge_true_1(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_ge_true_2(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_global_containing_addition(void *on_fns, void *g) {
	(void)on_fns;

	assert(((int32_t*)g)[0] == 5);

	free(g);
}

static void ok_globals(void *on_fns, void *g) {
	(void)on_fns;

	assert(game_fn_define_h_x == 42);

	assert(((int32_t*)g)[0] == 420);
	assert(((int32_t*)g)[1] == 1337);

	free(g);
}

static void ok_globals_1000(void *on_fns, void *g) {
	(void)on_fns;

	for (int32_t i = 0; i < 1000; i++) {
		assert(((int32_t*)g)[i] == i + 1);
	}

	free(g);
}

static void ok_globals_32(void *on_fns, void *g) {
	(void)on_fns;

	for (int32_t i = 0; i < 32; i++) {
		assert(((int32_t*)g)[i] == i + 1);
	}

	free(g);
}

static void ok_globals_64(void *on_fns, void *g) {
	(void)on_fns;

	for (int32_t i = 0; i < 64; i++) {
		assert(((int32_t*)g)[i] == i + 1);
	}

	free(g);
}

static void ok_gt_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_gt_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_helper_fn(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_helper_fn_overwriting_param(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 20);
	assert(game_fn_sin_x == 30.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_helper_fn_returning_void_has_no_return(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_helper_fn_returning_void_returns_void(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_i32_max(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 2147483647);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_i32_min(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -2147483648);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_i32_negated(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_identical_strings_are_shared(void *on_fns, void *g) {
	(void)on_fns;

	assert(streq(game_fn_define_q_a, "a"));
	assert(streq(game_fn_define_q_b, "b"));
	assert(streq(game_fn_define_q_c, "b"));

	free(g);
}

static void ok_if_false(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_if_true(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 3);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_le_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_le_true_1(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_le_true_2(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_lt_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_lt_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_max_args(void *on_fns, void *g) {
	assert(game_fn_mega_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_mega_call_count == 1);

	free(g);

	assert(game_fn_mega_f1 == 1.0f);
	assert(game_fn_mega_i1 == 21);
	assert(game_fn_mega_b1 == true);
	assert(game_fn_mega_f2 == 2.0f);
	assert(game_fn_mega_f3 == 3.0f);
	assert(game_fn_mega_f4 == 4.0f);
	assert(game_fn_mega_b2 == false);
	assert(game_fn_mega_i2 == 1337);
	assert(game_fn_mega_f5 == 5.0f);
	assert(game_fn_mega_f6 == 6.0f);
	assert(game_fn_mega_f7 == 7.0f);
	assert(game_fn_mega_f8 == 8.0f);
	assert(game_fn_mega_i3 == 8192);
	assert(streq(game_fn_mega_str, "foo"));

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_minimal(void *on_fns, void *g) {
	(void)on_fns;

	assert(game_fn_define_h_x == 42);

	free(g);
}

static void ok_multiplication_as_two_arguments(void *on_fns, void *g) {
	assert(game_fn_max_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_max_call_count == 1);

	free(g);

	assert(game_fn_max_x == 6);
	assert(game_fn_max_y == 20);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_ne_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_negate_parenthesized_expr(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -5);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_negative_literal(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_nested_break(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 3);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_nested_continue(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_ne_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_no_define_fields(void *on_fns, void *g) {
	(void)on_fns;

	free(g);
}

static void ok_no_on_fns(void *on_fns, void *g) {
	(void)on_fns;

	free(g);
}

static void ok_on_fn(void *on_fns, void *g) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_on_fn_calling_game_fn_nothing(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_on_fn_calling_game_fn_nothing_twice(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_on_fn_calling_game_fn_plt_order(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	assert(game_fn_magic_call_count == 0);
	assert(game_fn_initialize_call_count == 0);
	assert(game_fn_identity_call_count == 0);
	assert(game_fn_max_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);
	assert(game_fn_magic_call_count == 1);
	assert(game_fn_initialize_call_count == 1);
	assert(game_fn_identity_call_count == 1);
	assert(game_fn_max_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);
	assert(game_fn_identity_x == 69);
	assert(game_fn_max_x == 1337);
	assert(game_fn_max_y == 8192);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_on_fn_calling_helper_fns(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_on_fn_overwriting_param(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	assert(game_fn_sin_call_count == 0);
	((struct s_on_fns *)on_fns)->a(g, 2, 3.0f);
	assert(game_fn_initialize_call_count == 1);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 20);
	assert(game_fn_sin_x == 30.0f);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_on_fn_passing_argument_to_helper_fn(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_on_fn_passing_magic_to_initialize(void *on_fns, void *g) {
	assert(game_fn_magic_call_count == 0);
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_magic_call_count == 1);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_on_fn_three(void *on_fns, void *g) {
	((struct j_on_fns *)on_fns)->a(g);
	assert(streq(grug_on_fn_name, "on_a"));
	((struct j_on_fns *)on_fns)->b(g);
	assert(streq(grug_on_fn_name, "on_b"));
	((struct j_on_fns *)on_fns)->c(g);
	assert(streq(grug_on_fn_name, "on_c"));

	free(g);
}

static void ok_on_fn_three_unused_first(void *on_fns, void *g) {
	assert(((struct j_on_fns *)on_fns)->a == NULL);
	((struct j_on_fns *)on_fns)->b(g);
	assert(streq(grug_on_fn_name, "on_b"));
	((struct j_on_fns *)on_fns)->c(g);
	assert(streq(grug_on_fn_name, "on_c"));

	free(g);
}

static void ok_on_fn_three_unused_second(void *on_fns, void *g) {
	((struct j_on_fns *)on_fns)->a(g);
	assert(streq(grug_on_fn_name, "on_a"));
	assert(((struct j_on_fns *)on_fns)->b == NULL);
	((struct j_on_fns *)on_fns)->c(g);
	assert(streq(grug_on_fn_name, "on_c"));

	free(g);
}

static void ok_on_fn_three_unused_third(void *on_fns, void *g) {
	((struct j_on_fns *)on_fns)->a(g);
	assert(streq(grug_on_fn_name, "on_a"));
	((struct j_on_fns *)on_fns)->b(g);
	assert(streq(grug_on_fn_name, "on_b"));
	assert(((struct j_on_fns *)on_fns)->c == NULL);

	free(g);
}

static void ok_or_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_or_short_circuit(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_or_true_1(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_or_true_2(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_or_true_3(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_pass_string_argument_to_game_fn(void *on_fns, void *g) {
	assert(game_fn_say_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_say_call_count == 1);

	free(g);

	assert(streq(game_fn_say_message, "foo"));

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_pass_string_argument_to_helper_fn(void *on_fns, void *g) {
	assert(game_fn_say_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_say_call_count == 1);

	free(g);

	assert(streq(game_fn_say_message, "foo"));

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_remainder_negative_result(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -1);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_remainder_positive_result(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 1);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_resource_in_define(void *on_fns, void *g) {
	(void)on_fns;

	assert(streq(game_fn_define_u_sprite_path, "tests/ok/resource_in_define/foo.txt"));

	free(g);
}

static void ok_return(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_return_from_on_fn(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_return_with_no_value(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_stack_16_byte_alignment(void *on_fns, void *g) {
	assert(game_fn_nothing_aligned_call_count == 0);
	assert(game_fn_initialize_aligned_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_aligned_call_count == 1);
	assert(game_fn_initialize_aligned_call_count == 1);

	free(g);

	assert(game_fn_initialize_aligned_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_stack_16_byte_alignment_midway(void *on_fns, void *g) {
	assert(game_fn_magic_aligned_call_count == 0);
	assert(game_fn_initialize_aligned_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_magic_aligned_call_count == 1);
	assert(game_fn_initialize_aligned_call_count == 1);

	free(g);

	assert(game_fn_initialize_aligned_x == 42 + 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_string_and_on_fn(void *on_fns, void *g) {
	assert(streq(game_fn_define_p_x, "foo"));

	((struct p_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_string_eq_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_string_eq_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_string_eq_true_empty(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_string_ne_false(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_string_ne_false_empty(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_string_ne_true(void *on_fns, void *g) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_subtraction_negative_result(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -3);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_subtraction_positive_result(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 3);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_variable(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_variable_does_not_shadow_define_fn(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_variable_reassignment(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 69);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_variable_shadows_define_fn(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_variable_shadows_game_fn(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_variable_shadows_helper_fn(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_variable_shadows_on_fn_1(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_variable_shadows_on_fn_2(void *on_fns, void *g) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_variable_string_global(void *on_fns, void *g) {
	assert(game_fn_say_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_say_call_count == 1);

	free(g);

	assert(streq(game_fn_say_message, "foo"));

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_variable_string_local(void *on_fns, void *g) {
	assert(game_fn_say_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_say_call_count == 1);

	free(g);

	assert(streq(game_fn_say_message, "foo"));

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_void_function_early_return(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_while_false(void *on_fns, void *g) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void ok_write_to_global_variable(void *on_fns, void *g) {
	assert(game_fn_max_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_max_call_count == 1);

	free(g);

	assert(game_fn_max_x == 43);
	assert(game_fn_max_y == 69);

	assert(streq(grug_on_fn_name, "on_a"));
}

static void error_tests(void) {
	TEST_ERROR(assign_to_unknown_variable);
	TEST_ERROR(assignment_isnt_expression);
	TEST_ERROR(bool_unary_minus);
	TEST_ERROR(cant_add_strings);
	TEST_ERROR(cant_call_define_fn_1);
	TEST_ERROR(cant_call_define_fn_2);
	TEST_ERROR(cant_redefine_global);
	TEST_ERROR(define_fn_calls_fn);
	TEST_ERROR(define_fn_different_name);
	TEST_ERROR(define_fn_not_enough_arguments);
	TEST_ERROR(define_fn_only_one_max);
	TEST_ERROR(define_fn_uses_global_variable);
	TEST_ERROR(define_fn_was_not_declared);
	TEST_ERROR(f32_missing_digit_after_decimal_point);
	TEST_ERROR(game_fn_does_not_exist);
	TEST_ERROR(game_function_call_gets_wrong_arg_type);
	TEST_ERROR(game_function_call_less_args_expected);
	TEST_ERROR(game_function_call_more_args_expected);
	TEST_ERROR(game_function_call_no_args_expected);
	TEST_ERROR(global_variable_already_uses_local_variable_name);
	TEST_ERROR(global_variable_before_define);
	TEST_ERROR(global_variable_calls_fn);
	TEST_ERROR(global_variable_definition_cant_use_itself);
	TEST_ERROR(global_variable_definition_requires_value_i32);
	TEST_ERROR(global_variable_definition_requires_value_string);
	TEST_ERROR(global_variable_uses_global_variable);
	TEST_ERROR(helper_fn_does_not_exist);
	TEST_ERROR(helper_function_call_gets_wrong_arg_type);
	TEST_ERROR(helper_function_call_less_args_expected);
	TEST_ERROR(helper_function_call_more_args_expected);
	TEST_ERROR(helper_function_call_no_args_expected);
	TEST_ERROR(helper_function_different_return_value_expected);
	TEST_ERROR(helper_function_missing_return_statement);
	TEST_ERROR(helper_function_no_return_value_expected);
	TEST_ERROR(i32_logical_not);
	TEST_ERROR(i32_too_big);
	TEST_ERROR(i32_too_small);
	TEST_ERROR(local_variable_already_exists);
	TEST_ERROR(local_variable_definition_cant_use_itself);
	TEST_ERROR(missing_define_fn);
	TEST_ERROR(no_space_between_comment_character_and_comment);
	TEST_ERROR(on_fn_before_define);
	TEST_ERROR(on_fn_duplicate);
	TEST_ERROR(on_fn_was_not_declared_in_entity);
	TEST_ERROR(on_fn_wrong_order);
	TEST_ERROR(on_function_gets_wrong_arg_type);
	TEST_ERROR(on_function_less_args_expected);
	TEST_ERROR(on_function_more_args_expected);
	TEST_ERROR(on_function_no_args_expected);
	TEST_ERROR(on_function_no_return_value_expected);
	TEST_ERROR(pass_bool_to_i32_game_param);
	TEST_ERROR(pass_bool_to_i32_helper_param);
	TEST_ERROR(resource_type_for_global);
	TEST_ERROR(resource_type_for_helper_fn_argument);
	TEST_ERROR(resource_type_for_helper_fn_return_type);
	TEST_ERROR(resource_type_for_local);
	TEST_ERROR(resource_type_for_on_fn_argument);
	TEST_ERROR(string_pointer_arithmetic);
	TEST_ERROR(too_many_f32_arguments);
	TEST_ERROR(too_many_i32_arguments);
	TEST_ERROR(trailing_space_in_comment);
	TEST_ERROR(unclosed_double_quote);
	TEST_ERROR(unknown_variable);
	TEST_ERROR(unused_result);
	TEST_ERROR(variable_assignment_before_definition);
	TEST_ERROR(variable_definition_requires_value_i32);
	TEST_ERROR(variable_definition_requires_value_string);
	TEST_ERROR(variable_statement_missing_assignment);
	TEST_ERROR(variable_used_before_definition);
	TEST_ERROR(wrong_type_global_assignment);
	TEST_ERROR(wrong_type_global_reassignment);
	TEST_ERROR(wrong_type_local_assignment);
	TEST_ERROR(wrong_type_local_reassignment);
}

static void runtime_error_tests(void) {
	TEST_RUNTIME_ERROR(division_by_0);
	TEST_RUNTIME_ERROR(stack_overflow);
	TEST_RUNTIME_ERROR(time_limit_exceeded);
}

static void ok_tests(void) {
	TEST_OK(addition_as_argument, "d", 0);
	TEST_OK(addition_as_two_arguments, "d", 0);
	TEST_OK(addition_i32_wraparound, "d", 0);
	TEST_OK(addition_with_multiplication, "d", 0);
	TEST_OK(addition_with_multiplication_2, "d", 0);
	TEST_OK(and_false_1, "d", 0);
	TEST_OK(and_false_2, "d", 0);
	TEST_OK(and_false_3, "d", 0);
	TEST_OK(and_short_circuit, "d", 0);
	TEST_OK(and_true, "d", 0);
	TEST_OK(blocked_alrm, "d", 0);
	TEST_OK(bool_logical_not_false, "d", 0);
	TEST_OK(bool_logical_not_true, "d", 0);
	TEST_OK(bool_returned, "d", 0);
	TEST_OK(break, "d", 0);
	TEST_OK(calls_100, "d", 0);
	TEST_OK(calls_1000, "d", 0);
	TEST_OK(continue, "d", 0);
	TEST_OK(define_containing_addition, "b", 0);
	TEST_OK(define_containing_string, "k", 0);
	TEST_OK(define_with_eight_f32_fields, "t", 0);
	TEST_OK(define_with_six_fields, "m", 0);
	TEST_OK(define_with_six_i32_fields, "n", 0);
	TEST_OK(define_with_six_string_fields, "o", 0);
	TEST_OK(division_negative_result, "d", 0);
	TEST_OK(division_positive_result, "d", 0);
	TEST_OK(else_false, "d", 0);
	TEST_OK(else_true, "d", 0);
	TEST_OK(eq_false, "d", 0);
	TEST_OK(eq_true, "d", 0);
	TEST_OK(f32_addition, "d", 0);
	TEST_OK(f32_argument, "d", 0);
	TEST_OK(f32_division, "d", 0);
	TEST_OK(f32_eq_false, "d", 0);
	TEST_OK(f32_eq_true, "d", 0);
	TEST_OK(f32_ge_false, "d", 0);
	TEST_OK(f32_ge_true_1, "d", 0);
	TEST_OK(f32_ge_true_2, "d", 0);
	TEST_OK(f32_global_variable, "d", 4);
	TEST_OK(f32_gt_false, "d", 0);
	TEST_OK(f32_gt_true, "d", 0);
	TEST_OK(f32_le_false, "d", 0);
	TEST_OK(f32_le_true_1, "d", 0);
	TEST_OK(f32_le_true_2, "d", 0);
	TEST_OK(f32_local_variable, "d", 0);
	TEST_OK(f32_lt_false, "d", 0);
	TEST_OK(f32_lt_true, "d", 0);
	TEST_OK(f32_multiplication, "d", 0);
	TEST_OK(f32_ne_false, "d", 0);
	TEST_OK(f32_negated, "d", 0);
	TEST_OK(f32_ne_true, "d", 0);
	TEST_OK(f32_passed_to_helper_fn, "d", 0);
	TEST_OK(f32_passed_to_on_fn, "r", 0);
	TEST_OK(f32_passing_sin_to_cos, "d", 0);
	TEST_OK(f32_subtraction, "d", 0);
	TEST_OK(fibonacci, "d", 0);
	TEST_OK(ge_false, "d", 0);
	TEST_OK(ge_true_1, "d", 0);
	TEST_OK(ge_true_2, "d", 0);
	TEST_OK(global_containing_addition, "a", 4);
	TEST_OK(globals, "h", 8);
	TEST_OK(globals_1000, "a", 4000);
	TEST_OK(globals_32, "a", 128);
	TEST_OK(globals_64, "a", 256);
	TEST_OK(gt_false, "d", 0);
	TEST_OK(gt_true, "d", 0);
	TEST_OK(helper_fn, "d", 0);
	TEST_OK(helper_fn_overwriting_param, "d", 0);
	TEST_OK(helper_fn_returning_void_has_no_return, "d", 0);
	TEST_OK(helper_fn_returning_void_returns_void, "d", 0);
	TEST_OK(i32_max, "d", 0);
	TEST_OK(i32_min, "d", 0);
	TEST_OK(i32_negated, "d", 0);
	TEST_OK(identical_strings_are_shared, "q", 0);
	TEST_OK(if_false, "d", 0);
	TEST_OK(if_true, "d", 0);
	TEST_OK(le_false, "d", 0);
	TEST_OK(le_true_1, "d", 0);
	TEST_OK(le_true_2, "d", 0);
	TEST_OK(lt_false, "d", 0);
	TEST_OK(lt_true, "d", 0);
	TEST_OK(max_args, "d", 0);
	TEST_OK(minimal, "h", 0);
	TEST_OK(multiplication_as_two_arguments, "d", 0);
	TEST_OK(ne_false, "d", 0);
	TEST_OK(negate_parenthesized_expr, "d", 0);
	TEST_OK(negative_literal, "d", 0);
	TEST_OK(nested_break, "d", 0);
	TEST_OK(nested_continue, "d", 0);
	TEST_OK(ne_true, "d", 0);
	TEST_OK(no_define_fields, "d", 0);
	TEST_OK(no_on_fns, "a", 0);
	TEST_OK(on_fn, "d", 0);
	TEST_OK(on_fn_calling_game_fn_nothing, "d", 0);
	TEST_OK(on_fn_calling_game_fn_nothing_twice, "d", 0);
	TEST_OK(on_fn_calling_game_fn_plt_order, "d", 0);
	TEST_OK(on_fn_calling_helper_fns, "d", 0);
	TEST_OK(on_fn_overwriting_param, "s", 0);
	TEST_OK(on_fn_passing_argument_to_helper_fn, "d", 0);
	TEST_OK(on_fn_passing_magic_to_initialize, "d", 0);
	TEST_OK(on_fn_three, "j", 0);
	TEST_OK(on_fn_three_unused_first, "j", 0);
	TEST_OK(on_fn_three_unused_second, "j", 0);
	TEST_OK(on_fn_three_unused_third, "j", 0);
	TEST_OK(or_false, "d", 0);
	TEST_OK(or_short_circuit, "d", 0);
	TEST_OK(or_true_1, "d", 0);
	TEST_OK(or_true_2, "d", 0);
	TEST_OK(or_true_3, "d", 0);
	TEST_OK(pass_string_argument_to_game_fn, "d", 0);
	TEST_OK(pass_string_argument_to_helper_fn, "d", 0);
	TEST_OK(remainder_negative_result, "d", 0);
	TEST_OK(remainder_positive_result, "d", 0);
	TEST_OK(resource_in_define, "u", 0);
	TEST_OK(return, "d", 0);
	TEST_OK(return_from_on_fn, "d", 0);
	TEST_OK(return_with_no_value, "d", 0);
	TEST_OK(stack_16_byte_alignment, "d", 0);
	TEST_OK(stack_16_byte_alignment_midway, "d", 0);
	TEST_OK(string_and_on_fn, "p", 0);
	TEST_OK(string_eq_false, "d", 0);
	TEST_OK(string_eq_true, "d", 0);
	TEST_OK(string_eq_true_empty, "d", 0);
	TEST_OK(string_ne_false, "d", 0);
	TEST_OK(string_ne_false_empty, "d", 0);
	TEST_OK(string_ne_true, "d", 0);
	TEST_OK(subtraction_negative_result, "d", 0);
	TEST_OK(subtraction_positive_result, "d", 0);
	TEST_OK(variable, "d", 0);
	TEST_OK(variable_does_not_shadow_define_fn, "d", 0);
	TEST_OK(variable_reassignment, "d", 0);
	TEST_OK(variable_shadows_define_fn, "d", 0);
	TEST_OK(variable_shadows_game_fn, "d", 0);
	TEST_OK(variable_shadows_helper_fn, "d", 0);
	TEST_OK(variable_shadows_on_fn_1, "d", 0);
	TEST_OK(variable_shadows_on_fn_2, "j", 0);
	TEST_OK(variable_string_global, "d", 8);
	TEST_OK(variable_string_local, "d", 0);
	TEST_OK(void_function_early_return, "d", 0);
	TEST_OK(while_false, "d", 0);
	TEST_OK(write_to_global_variable, "d", 8);
}

int main(void) {
	error_tests();
	runtime_error_tests();
	ok_tests();

	printf("\nAll tests passed!\n");
}
