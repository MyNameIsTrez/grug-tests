#define _XOPEN_SOURCE 700

#include "grug/grug.h"

#include "mod_api.h"

#include <assert.h>
#include <dlfcn.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Forward declaration, since grug.h doesn't declare it
bool grug_test_regenerate_dll(char *grug_file_path, char *dll_path, char *mod);

#define MAX_DYNSTR_LENGTH 420420

typedef int64_t i64;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

struct test_data {
	bool run;
	void *on_fns;
	void *g;
	size_t resources_size;
	char **resources;
	size_t entities_size;
	char **entities;
	char **entity_types;
	void *dll;
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
static size_t game_fn_spawn_call_count;
static size_t game_fn_has_resource_call_count;
static size_t game_fn_has_entity_call_count;
static size_t game_fn_has_string_call_count;

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

#ifdef __x86_64__
	asm("mov %%rsp, %0\n\t" : "=r" (rsp));
#elif __aarch64__
	asm("mov %0, sp\n\t" : "=r" (rsp));
#else
#error Unsupported or unrecognized architecture
#endif

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

#ifdef __x86_64__
	asm("mov %%rsp, %0\n\t" : "=r" (rsp));
#elif __aarch64__
	asm("mov %0, sp\n\t" : "=r" (rsp));
#else
#error Unsupported or unrecognized architecture
#endif

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

#ifdef __x86_64__
	asm("mov %%rsp, %0\n\t" : "=r" (rsp));
#elif __aarch64__
	asm("mov %0, sp\n\t" : "=r" (rsp));
#else
#error Unsupported or unrecognized architecture
#endif

	#pragma GCC diagnostic pop

	// We need this in order to ensure that the C compiler will 16-byte align
	// this function with a function prologue, cause we assert that the rsp is divisible by 16 after the function prologue
	printf(":)\n");

	assert((rsp & 0xf) == 0);
}
static char *game_fn_spawn_name;
void game_fn_spawn(char *name) {
	game_fn_spawn_call_count++;

	game_fn_spawn_name = name;
}
static char *game_fn_has_resource_path;
bool game_fn_has_resource(char *path) {
	game_fn_has_resource_call_count++;

	game_fn_has_resource_path = path;

	return true;
}
static char *game_fn_has_entity_name;
bool game_fn_has_entity(char *name) {
	game_fn_has_entity_call_count++;

	game_fn_has_entity_name = name;

	return true;
}
static char *game_fn_has_string_str;
bool game_fn_has_string(char *str) {
	game_fn_has_string_call_count++;

	game_fn_has_string_str = str;

	return true;
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
static char *game_fn_define_v_foo;
static char *game_fn_define_v_bar;
void game_fn_define_v(char *foo, char *bar) {
	game_fn_define_v_foo = foo;
	game_fn_define_v_bar = bar;
}
static char *game_fn_define_w_sprite_path;
void game_fn_define_w(char *sprite_path) {
	game_fn_define_w_sprite_path = sprite_path;
}
static char *game_fn_define_x_projectile;
void game_fn_define_x(char *projectile) {
	game_fn_define_x_projectile = projectile;
}
static char *game_fn_define_y_foo;
static char *game_fn_define_y_bar;
void game_fn_define_y(char *foo, char *bar) {
	game_fn_define_y_foo = foo;
	game_fn_define_y_bar = bar;
}
static char *game_fn_define_z_projectile;
void game_fn_define_z(char *projectile) {
	game_fn_define_z_projectile = projectile;
}
static char *game_fn_define_a2_sprite_path;
static char *game_fn_define_a2_projectile;
void game_fn_define_a2(char *sprite_path, char *projectile) {
	game_fn_define_a2_sprite_path = sprite_path;
	game_fn_define_a2_projectile = projectile;
}
static char *game_fn_define_b2_sprite_path;
static char *game_fn_define_b2_projectile;
void game_fn_define_b2(char *sprite_path, char *projectile) {
	game_fn_define_b2_sprite_path = sprite_path;
	game_fn_define_b2_projectile = projectile;
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
	game_fn_spawn_call_count = 0;
	game_fn_has_resource_call_count = 0;
	game_fn_has_entity_call_count = 0;
	game_fn_has_string_call_count = 0;
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

#define MAX_WHITELISTED_TESTS 420420
static char *whitelisted_tests[MAX_WHITELISTED_TESTS];
static size_t whitelisted_tests_size;
static bool is_whitelisted_test(char *name) {
	for (size_t i = 0; i < whitelisted_tests_size; i++) {
		if (streq(whitelisted_tests[i], name)) {
			return true;
		}
	}
	return false;
}

#define TEST_ERROR(test_name) {\
	if (whitelisted_tests_size == 0 || is_whitelisted_test(#test_name)) {\
		test_error(\
			#test_name,\
			"tests/err/"#test_name"/input.grug",\
			"tests/err/"#test_name"/expected_error.txt",\
			"tests/err/"#test_name"/results",\
			"tests/err/"#test_name"/results/output.so",\
			"tests/err/"#test_name"/results/grug_output.txt",\
			"tests/err/"#test_name"/results/failed"\
		);\
	}\
}

#define TEST_RUNTIME_ERROR(test_name, expected_define_type, expected_globals_size) {\
	if (whitelisted_tests_size == 0 || is_whitelisted_test(#test_name)) {\
		struct test_data data = runtime_error_prologue(\
			#test_name,\
			"tests/err_runtime/"#test_name"/input.grug",\
			"tests/err_runtime/"#test_name"/input.s",\
			"tests/err_runtime/"#test_name"/expected_error.txt",\
			"tests/err_runtime/"#test_name"/results",\
			"tests/err_runtime/"#test_name"/results/output.so",\
			"tests/err_runtime/"#test_name"/results/expected.so",\
			"tests/err_runtime/"#test_name"/results/expected.o",\
			"tests/err_runtime/"#test_name"/results/expected.hex",\
			"tests/err_runtime/"#test_name"/results/expected_elf.log",\
			"tests/err_runtime/"#test_name"/results/expected_objdump.log",\
			"tests/err_runtime/"#test_name"/results/failed",\
			expected_define_type,\
			expected_globals_size\
		);\
		if (data.run) {\
			runtime_error_reason = NULL;\
			signal_handler_called = false;\
			had_runtime_error = false;\
			runtime_error_on_fn_name = NULL;\
			runtime_error_on_fn_path = NULL;\
			runtime_error_##test_name(data.on_fns, data.g, data.resources_size, data.resources, data.entities_size, data.entities, data.entity_types);\
			runtime_error_epilogue(\
				"tests/err_runtime/"#test_name"/input.grug",\
				"tests/err_runtime/"#test_name"/expected_error.txt",\
				"tests/err_runtime/"#test_name"/results/output.so",\
				"tests/err_runtime/"#test_name"/results/expected.so",\
				"tests/err_runtime/"#test_name"/results/output.hex",\
				"tests/err_runtime/"#test_name"/results/output_elf.log",\
				"tests/err_runtime/"#test_name"/results/output_objdump.log",\
				"tests/err_runtime/"#test_name"/results/dump.json",\
				"tests/err_runtime/"#test_name"/results/applied.grug",\
				"tests/err_runtime/"#test_name"/results/failed"\
			);\
		}\
		if (data.dll && dlclose(data.dll)) {\
			handle_dlerror("dlclose");\
		}\
	}\
}

#define TEST_OK(test_name, expected_define_type, expected_globals_size) {\
	if (whitelisted_tests_size == 0 || is_whitelisted_test(#test_name)) {\
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
			ok_##test_name(data.on_fns, data.g, data.resources_size, data.resources, data.entities_size, data.entities, data.entity_types);\
			ok_epilogue(\
				"tests/ok/"#test_name"/input.grug",\
				"tests/ok/"#test_name"/results/output.so",\
				"tests/ok/"#test_name"/results/expected.so",\
				"tests/ok/"#test_name"/results/output.hex",\
				"tests/ok/"#test_name"/results/output_elf.log",\
				"tests/ok/"#test_name"/results/output_objdump.log",\
				"tests/ok/"#test_name"/results/dump.json",\
				"tests/ok/"#test_name"/results/applied.grug",\
				"tests/ok/"#test_name"/results/failed"\
			);\
		}\
		if (data.dll && dlclose(data.dll)) {\
			handle_dlerror("dlclose");\
		}\
	}\
}

static size_t read_file(char *path, uint8_t *bytes) {
	FILE *f = fopen(path, "r");
	check_null(f, "fopen");

	check(fseek(f, 0, SEEK_END), "fseek");

	long ftell_result = ftell(f);
	check(ftell_result, "ftell");
	size_t len = ftell_result;

	check(fseek(f, 0, SEEK_SET), "fseek");

	if (fread(bytes, len, 1, f) < len && ferror(f)) {
		printf("fread error\n");
		exit(EXIT_FAILURE);
	}

    if (fclose(f) == EOF) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}

	return len;
}

static void *get(void *dll, char *label) {
	void *p = dlsym(dll, label);
	if (!p) {
		printf("dlsym: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}
	return p;
}

static void wait_on_child(char *child_name) {
	int status;
	check(wait(&status), "wait");

	if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		printf("child \"%s\" unexpectedly exited with status %d\n", child_name, WEXITSTATUS(status));
		exit(EXIT_FAILURE);
	} else if (WIFSIGNALED(status)) {
		printf("child \"%s\" killed by signal %d\n", child_name, WTERMSIG(status));
		exit(EXIT_FAILURE);
	} else if (WIFSTOPPED(status)) {
		printf("child \"%s\" stopped by signal %d\n", child_name, WSTOPSIG(status));
		exit(EXIT_FAILURE);
	} else if (WIFCONTINUED(status)) {
		printf("child \"%s\" continued\n", child_name);
		exit(EXIT_FAILURE);
	}
}

static void run_and_write(char *const *argv, char *written_path) {
	pid_t pid = fork();
	check(pid, "fork");

	if (pid == 0) {
		int fd = open(written_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
		check(fd, "open");

		dup2(fd, STDOUT_FILENO); // Redirect stdout to written_path

		close(fd);

		execvp(argv[0], argv);
		printf("execvp: %s: %s\n", argv[0], strerror(errno));
		exit(EXIT_FAILURE);
	}

	wait_on_child(argv[0]);
}

static void run(char *const *argv) {
	pid_t pid = fork();
	check(pid, "fork");

	if (pid == 0) {
		execvp(argv[0], argv);
		printf("execvp: %s: %s\n", argv[0], strerror(errno));
		exit(EXIT_FAILURE);
	}

	wait_on_child(argv[0]);
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

		execvp("xxd", (char *[]){"xxd", dll_path, NULL});
		printf("execvp: xxd: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	wait_on_child("xxd");

	run_and_write((char *[]){"readelf", "--wide", "-a", dll_path, NULL}, readelf_path);

	run_and_write((char *[]){"objdump", "-D", dll_path, "-Mintel", NULL}, objdump_path);
}

static bool newer(char *path1, char *path2) {
	struct stat s1;
	if (stat(path1, &s1) == -1) {
		if (errno != ENOENT) {
			printf("path1: \"%s\"\n", path1);
			perror("stat");
			exit(EXIT_FAILURE);
		}
		return false;
	}

	struct stat s2;
	if (stat(path2, &s2) < 0) {
		printf("path2: \"%s\"\n", path2);
		perror("stat");
		exit(EXIT_FAILURE);
	}

	return s1.st_mtime >= s2.st_mtime;
}

static char *get_expected_error(char *expected_error_path) {
	static char expected_error[420];
	size_t expected_error_len = read_file(expected_error_path, (uint8_t *)expected_error);

	if (expected_error[expected_error_len - 1] == '\n') {
		expected_error_len--;
		if (expected_error[expected_error_len - 1] == '\r') {
			expected_error_len--;
		}
	}

	expected_error[expected_error_len] = '\0';

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

	assert(grug_test_regenerate_dll(grug_path, output_dll_path, "err"));

	FILE *f = fopen(grug_output_path, "w");

	size_t grug_error_msg_len = strlen(grug_error.msg);

	if (fwrite(grug_error.msg, grug_error_msg_len, 1, f) == 0) {
		printf("fwrite error\n");
		exit(EXIT_FAILURE);
	}

    if (fclose(f) == EOF) {
		perror("fclose");
		exit(EXIT_FAILURE);
		exit(EXIT_FAILURE);
	}

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

static void diff_dump_and_apply(
	char *grug_path,
	char *dump_path,
	char *applied_path
) {
	if (grug_dump_file_to_json(grug_path, dump_path)) {
		printf("Failed to dump file AST: %s: %s (detected in grug.c:%d)\n", grug_error.path, grug_error.msg, grug_error.grug_c_line_number);
		exit(EXIT_FAILURE);
	}

	if (grug_generate_file_from_json(dump_path, applied_path)) {
		printf("Failed to apply file AST: %s: %s (detected in grug.c:%d)\n", grug_error.path, grug_error.msg, grug_error.grug_c_line_number);
		exit(EXIT_FAILURE);
	}

	static uint8_t grug_path_bytes[420420];
	size_t grug_path_bytes_len = read_file(grug_path, grug_path_bytes);
	grug_path_bytes[grug_path_bytes_len] = '\0';

	static uint8_t applied_path_bytes[420420];
	size_t applied_path_bytes_len = read_file(applied_path, applied_path_bytes);
	applied_path_bytes[applied_path_bytes_len] = '\0';

	if (grug_path_bytes_len != applied_path_bytes_len || memcmp(grug_path_bytes, applied_path_bytes, grug_path_bytes_len) != 0) {
		printf("\nThe output differs from the expected output.\n");
		printf("Output:\n");
		printf("%s\n", applied_path_bytes);

		printf("Expected:\n");
		printf("%s\n", grug_path_bytes);

		exit(EXIT_FAILURE);
	}
}

static void generate_and_compare_output_dll(
	char *grug_path,
	char *output_dll_path,
	char *expected_dll_path,
	char *output_xxd_path,
	char *output_readelf_path,
	char *output_objdump_path,
	char *dump_path,
	char *applied_path,
	char *failed_file_path
) {
	if (grug_test_regenerate_dll(grug_path, output_dll_path, "ok")) {
		printf("The test wasn't supposed to print anything, but did:\n");
		printf("----\n");
		printf("%s\n", grug_error.msg);
		printf("----\n");

		exit(EXIT_FAILURE);
	}

	output_dll_info(output_dll_path, output_xxd_path, output_readelf_path, output_objdump_path);

	diff_dump_and_apply(grug_path, dump_path, applied_path);

	static uint8_t output_dll_bytes[420420];
	size_t output_dll_bytes_len = read_file(output_dll_path, output_dll_bytes);

	static uint8_t expected_dll_bytes[420420];
	size_t expected_dll_bytes_len = read_file(expected_dll_path, expected_dll_bytes);

	if (output_dll_bytes_len != expected_dll_bytes_len || memcmp(output_dll_bytes, expected_dll_bytes, expected_dll_bytes_len) != 0) {
		printf("\nThe OK test's DLL bytes output differs from the expected output.\n");

		if (output_dll_bytes_len == expected_dll_bytes_len) {
			printf("The output DLL bytes length matches the expected length.\n");
		} else {
			printf("The output DLL bytes length was %zu, while the expected length was %zu.\n", output_dll_bytes_len, expected_dll_bytes_len);
		}

		exit(EXIT_FAILURE);
	}

	unlink(failed_file_path);
}

static char *runtime_error_reason = NULL;

static void runtime_error_epilogue(
	char *grug_path,
	char *expected_error_path,
	char *output_dll_path,
	char *expected_dll_path,
	char *output_xxd_path,
	char *output_readelf_path,
	char *output_objdump_path,
	char *dump_path,
	char *applied_path,
	char *failed_file_path
) {
	size_t grug_error_msg_len = strlen(runtime_error_reason);

	char *expected_error = get_expected_error(expected_error_path);
	size_t expected_error_len = strlen(expected_error);

	if (expected_error_len != grug_error_msg_len || memcmp(runtime_error_reason, expected_error, expected_error_len) != 0) {
		printf("\nThe error message differs from the expected error message.\n");
		printf("Output:\n");
		printf("%s\n", runtime_error_reason);

		printf("Expected:\n");
		printf("%s\n", expected_error);

		exit(EXIT_FAILURE);
	}

	generate_and_compare_output_dll(grug_path, output_dll_path, expected_dll_path, output_xxd_path, output_readelf_path, output_objdump_path, dump_path, applied_path, failed_file_path);
}

static void handle_dlerror(char *function_name) {
	char *err = dlerror();
	if (!err) {
		printf("dlerror() was asked to find an error string for %s(), but it couldn't find one", function_name);
		exit(EXIT_FAILURE);
	}

	printf("%s: %s\n", function_name, err);
	exit(EXIT_FAILURE);
}

static void regenerate_expected_dll(
	char *grug_path,
	char *nasm_path,
	char *expected_dll_path,
	char *nasm_o_path,
	char *expected_xxd_path,
	char *expected_readelf_path,
	char *expected_objdump_path
) {
#ifdef __x86_64__
	run((char *[]){"nasm", nasm_path, "-felf64", "-O0", "-o", nasm_o_path, NULL});
	run((char *[]){"ld", nasm_o_path, "-o", expected_dll_path, "-shared", "--hash-style=sysv", NULL});
#elif __aarch64__
	run((char *[]){"nasm", nasm_path, "-fmacho64", "-O0", "-o", nasm_o_path, NULL});
	run((char *[]){"ld", nasm_o_path, "-o", expected_dll_path, "-dylib", NULL});
#else
#error Unsupported or unrecognized architecture
#endif

	static char redefine_sym[420];

	size_t nasm_path_len = strlen(nasm_path);
	size_t grug_path_len = strlen(grug_path);

	assert(nasm_path_len + 1 + grug_path_len + 1 <= sizeof(redefine_sym));

	memcpy(redefine_sym, nasm_path, nasm_path_len);
	redefine_sym[nasm_path_len] = '=';
	memcpy(redefine_sym + nasm_path_len + 1, grug_path, grug_path_len);

	run((char *[]){"objcopy", expected_dll_path, "--redefine-sym", redefine_sym, NULL});

	output_dll_info(expected_dll_path, expected_xxd_path, expected_readelf_path, expected_objdump_path);
}

static struct test_data get_expected_test_data(
	char *grug_path,
	char *nasm_path,
	char *results_path,
	char *expected_dll_path,
	char *nasm_o_path,
	char *expected_xxd_path,
	char *expected_readelf_path,
	char *expected_objdump_path,
	char *failed_file_path,
	char *expected_define_type,
	size_t expected_globals_size
) {
	reset_call_counts();

	rm_rf(results_path);
	make_results_dir(results_path);

	create_failed_file(failed_file_path);

	regenerate_expected_dll(grug_path, nasm_path, expected_dll_path, nasm_o_path, expected_xxd_path, expected_readelf_path, expected_objdump_path);

	void *dll = dlopen(expected_dll_path, RTLD_NOW);
	if (!dll) {
		handle_dlerror("dlopen");
	}

	assert(streq(get(dll, "define_type"), expected_define_type));

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	grug_define_fn_t define = get(dll, "define");
	define();

	size_t globals_size = *(size_t *)get(dll, "globals_size");
	assert(globals_size == expected_globals_size);

	void *g = malloc(globals_size);
	grug_init_globals_fn_t init_globals = get(dll, "init_globals");
	init_globals(g);
	#pragma GCC diagnostic pop

	void *on_fns = dlsym(dll, "on_fns");

	size_t *resources_size_ptr = get(dll, "resources_size");
	char **resources = NULL;
	if (*resources_size_ptr > 0) {
		resources = get(dll, "resources");
	} else {
		assert(!dlsym(dll, "resources"));
	}

	size_t *entities_size_ptr = get(dll, "entities_size");
	char **entities = NULL;
	char **entity_types = NULL;
	if (*entities_size_ptr > 0) {
		entities = get(dll, "entities");
		entity_types = get(dll, "entity_types");
	} else {
		assert(!dlsym(dll, "entities"));
		assert(!dlsym(dll, "entity_types"));
	}

	return (struct test_data){
		.run = true,
		.on_fns = on_fns,
		.g = g,
		.resources_size = *resources_size_ptr,
		.resources = resources,
		.entities_size = *entities_size_ptr,
		.entities = entities,
		.entity_types = entity_types,
		.dll = dll,
	};
}

static struct test_data runtime_error_prologue(
	char *test_name,
	char *grug_path,
	char *nasm_path,
	char *expected_error_path,
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
	 && newer(output_dll_path, expected_error_path)
	 && newer(output_dll_path, expected_dll_path)
	 && newer(output_dll_path, "mod_api.h")
	 && newer(output_dll_path, "mod_api.json")
	 && newer(output_dll_path, "tests.sh")
	 && newer(output_dll_path, "tests.out")
	) {
		printf("Skipping tests/err_runtime/%s...\n", test_name);
		return (struct test_data){.run=false};
	}

	printf("Running tests/err_runtime/%s...\n", test_name);

	return get_expected_test_data(grug_path, nasm_path, results_path, expected_dll_path, nasm_o_path, expected_xxd_path, expected_readelf_path, expected_objdump_path, failed_file_path, expected_define_type, expected_globals_size);
}

static bool signal_handler_called = false;
static void signal_handler(int sig) {
	(void)sig;
	signal_handler_called = true;
}

static bool had_runtime_error = false;
static char *runtime_error_on_fn_name = NULL;
static char *runtime_error_on_fn_path = NULL;
static void runtime_error_handler(char *reason, enum grug_runtime_error_type type, char *on_fn_name, char *on_fn_path) {
	(void)type;

	had_runtime_error = true;

	runtime_error_reason = reason;

	runtime_error_on_fn_name = on_fn_name;
	runtime_error_on_fn_path = on_fn_path;
}

static void runtime_error_time_limit_exceeded(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	signal(SIGALRM, signal_handler);

	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	raise(SIGALRM);
	assert(signal_handler_called);

	free(g);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/time_limit_exceeded/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_division_by_0(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	signal(SIGFPE, signal_handler);

	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	raise(SIGFPE);
	assert(signal_handler_called);

	free(g);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/division_by_0/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_stack_overflow(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	signal(SIGSEGV, signal_handler);

	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	raise(SIGSEGV);
	assert(signal_handler_called);

	free(g);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/stack_overflow/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_epilogue(
	char *grug_path,
	char *output_dll_path,
	char *expected_dll_path,
	char *output_xxd_path,
	char *output_readelf_path,
	char *output_objdump_path,
	char *dump_path,
	char *applied_path,
	char *failed_file_path
) {
	generate_and_compare_output_dll(grug_path, output_dll_path, expected_dll_path, output_xxd_path, output_readelf_path, output_objdump_path, dump_path, applied_path, failed_file_path);
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

	return get_expected_test_data(grug_path, nasm_path, results_path, expected_dll_path, nasm_o_path, expected_xxd_path, expected_readelf_path, expected_objdump_path, failed_file_path, expected_define_type, expected_globals_size);
}

static void ok_addition_as_argument(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 3);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_addition_as_two_arguments(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_max_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_max_call_count == 1);

	free(g);

	assert(game_fn_max_x == 3);
	assert(game_fn_max_y == 9);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_addition_i32_wraparound(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == INT32_MIN);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_addition_with_multiplication(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 14);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_addition_with_multiplication_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 10);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_and_false_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_and_false_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_and_false_3(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_and_short_circuit(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_and_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_blocked_alrm(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_blocked_alrm_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_blocked_alrm_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_bool_logical_not_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_bool_logical_not_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_bool_returned(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_is_happy_call_count == 0);
	assert(game_fn_is_friday_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_is_happy_call_count == 1);
	assert(game_fn_is_friday_call_count == 1);

	free(g);

	assert(game_fn_set_is_happy_is_happy == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_break(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 3);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_calls_100(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 100);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_calls_1000(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1000);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_above_block(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_above_block_twice(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_above_define_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_above_define_fn_twice(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_above_globals(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(((int32_t*)g)[0] == 420);
	assert(((int32_t*)g)[1] == 1337);
	assert(((int32_t*)g)[2] == 100);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_above_helper_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_above_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_between_globals(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(((int32_t*)g)[0] == 420);
	assert(((int32_t*)g)[1] == 1337);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_between_statements(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_lone_block(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_lone_block_at_end(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_lone_global(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_continue(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_define(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_define_h_x == 42);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_define_containing_addition(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_define_b_x == 3);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_define_containing_string(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_define_k_age == 42);
	assert(streq(game_fn_define_k_name, "foo"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_define_with_eight_f32_fields(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
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

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_define_with_six_fields(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_define_m_w == 42);
	assert(streq(game_fn_define_m_group, "bar"));
	assert(game_fn_define_m_b1 == true);
	assert(streq(game_fn_define_m_name, "foobar"));
	assert(game_fn_define_m_b2 == false);
	assert(game_fn_define_m_z == 1337);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_define_with_six_i32_fields(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_define_n_u == 1);
	assert(game_fn_define_n_v == 2);
	assert(game_fn_define_n_w == 3);
	assert(game_fn_define_n_x == 4);
	assert(game_fn_define_n_y == 5);
	assert(game_fn_define_n_z == 6);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_define_with_six_string_fields(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_o_u, "u"));
	assert(streq(game_fn_define_o_v, "v"));
	assert(streq(game_fn_define_o_w, "w"));
	assert(streq(game_fn_define_o_x, "x"));
	assert(streq(game_fn_define_o_y, "y"));
	assert(streq(game_fn_define_o_z, "z"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_division_negative_result(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -2);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_division_positive_result(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 2);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_else_after_else_if_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_else_after_else_if_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 3);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_else_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_else_if_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_else_if_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 3);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_else_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 3);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_empty_line(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_entity_and_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(streq(game_fn_define_z_projectile, "ok:foo"));

	((struct z_on_fns *)on_fns)->a(g, 42);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 1);
	assert(streq(entities[0], "ok:foo"));
	assert(streq(entity_types[0], ""));
}

static void ok_entity_and_resource_as_subexpression(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_has_resource_call_count == 0);
	assert(game_fn_has_entity_call_count == 0);
	assert(game_fn_has_string_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_has_resource_call_count == 1);
	assert(game_fn_has_entity_call_count == 1);
	assert(game_fn_has_string_call_count == 1);

	free(g);

	assert(streq(game_fn_has_resource_path, "tests/ok/entity_and_resource_as_subexpression/foo.txt"));
	assert(streq(game_fn_has_entity_name, "ok:baz"));
	assert(streq(game_fn_has_string_str, "bar"));

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/entity_and_resource_as_subexpression/foo.txt"));

	assert(entities_size == 1);
	assert(streq(entities[0], "ok:baz"));
	assert(streq(entity_types[0], ""));
}

static void ok_entity_duplicate(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_y_foo, "ok:foo"));
	assert(streq(game_fn_define_y_bar, "ok:foo"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 2);
	assert(streq(entities[0], "ok:foo"));
	assert(streq(entity_types[0], ""));
	assert(streq(entities[1], "ok:foo"));
	assert(streq(entity_types[1], ""));
}

static void ok_entity_in_define(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_x_projectile, "ok:foo"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 1);
	assert(streq(entities[0], "ok:foo"));
	assert(streq(entity_types[0], ""));
}

static void ok_entity_in_define_with_mod_specified(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_x_projectile, "wow:foo"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 1);
	assert(streq(entities[0], "wow:foo"));
	assert(streq(entity_types[0], ""));
}

static void ok_entity_twice(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_y_foo, "ok:foo"));
	assert(streq(game_fn_define_y_bar, "ok:bar"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 2);
	assert(streq(entities[0], "ok:bar"));
	assert(streq(entity_types[0], ""));
	assert(streq(entities[1], "ok:foo"));
	assert(streq(entity_types[1], ""));
}

static void ok_eq_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_eq_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_addition(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 6.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_argument(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 4.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_division(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 0.5f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_eq_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_eq_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_ge_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_ge_true_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_ge_true_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_global_variable(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 4.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_gt_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_gt_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_le_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_le_true_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_le_true_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_local_variable(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 4.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_lt_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_lt_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_multiplication(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 8.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_ne_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_negated(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == -4.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_ne_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_passed_to_helper_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 42.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_passed_to_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	((struct r_on_fns *)on_fns)->a(g, 42.0f);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 42.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_passing_sin_to_cos(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	assert(game_fn_cos_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);
	assert(game_fn_cos_call_count == 1);

	free(g);

	assert(game_fn_sin_x == 4.0f);
	assert(game_fn_cos_x == sinf(4.0f));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_f32_subtraction(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_sin_x == -2.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_fibonacci(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 55);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_ge_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_ge_true_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_ge_true_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_global_containing_addition(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(((int32_t*)g)[0] == 5);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_globals(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_define_h_x == 42);

	assert(((int32_t*)g)[0] == 420);
	assert(((int32_t*)g)[1] == 1337);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_globals_1000(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	for (int32_t i = 0; i < 1000; i++) {
		assert(((int32_t*)g)[i] == i + 1);
	}

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_globals_32(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	for (int32_t i = 0; i < 32; i++) {
		assert(((int32_t*)g)[i] == i + 1);
	}

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_globals_64(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	for (int32_t i = 0; i < 64; i++) {
		assert(((int32_t*)g)[i] == i + 1);
	}

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_gt_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_gt_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_helper_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_helper_fn_overwriting_param(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	assert(game_fn_sin_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 20);
	assert(game_fn_sin_x == 30.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_helper_fn_returning_void_has_no_return(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_helper_fn_returning_void_returns_void(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_i32_max(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 2147483647);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_i32_min(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -2147483648);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_i32_negated(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_identical_strings_are_shared(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_q_a, "a"));
	assert(streq(game_fn_define_q_b, "b"));
	assert(streq(game_fn_define_q_c, "b"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_if_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_if_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 3);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_le_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_le_true_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_le_true_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_lt_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_lt_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_max_args(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
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

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_minimal(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_multiplication_as_two_arguments(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_max_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_max_call_count == 1);

	free(g);

	assert(game_fn_max_x == 6);
	assert(game_fn_max_y == 20);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_ne_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_negate_parenthesized_expr(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -5);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_negative_literal(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_nested_break(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 3);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_nested_continue(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_ne_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_no_define_fields(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_no_on_fns(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_no_empty_line_between_globals(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(((int32_t*)g)[0] == 420);
	assert(((int32_t*)g)[1] == 1337);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_no_empty_line_between_statements(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_calling_game_fn_nothing(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_calling_game_fn_nothing_twice(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_calling_game_fn_plt_order(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
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

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_calling_helper_fns(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_overwriting_param(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	assert(game_fn_sin_call_count == 0);
	((struct s_on_fns *)on_fns)->a(g, 2, 3.0f);
	assert(game_fn_initialize_call_count == 1);
	assert(game_fn_sin_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 20);
	assert(game_fn_sin_x == 30.0f);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_passing_argument_to_helper_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_passing_magic_to_initialize(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_magic_call_count == 0);
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_magic_call_count == 1);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_three(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);
	((struct j_on_fns *)on_fns)->b(g);
	((struct j_on_fns *)on_fns)->c(g);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_three_unused_first(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(((struct j_on_fns *)on_fns)->a == NULL);
	((struct j_on_fns *)on_fns)->b(g);
	((struct j_on_fns *)on_fns)->c(g);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_three_unused_second(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);
	assert(((struct j_on_fns *)on_fns)->b == NULL);
	((struct j_on_fns *)on_fns)->c(g);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_three_unused_third(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);
	((struct j_on_fns *)on_fns)->b(g);
	assert(((struct j_on_fns *)on_fns)->c == NULL);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_usage_is_optional(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(((struct d_on_fns *)on_fns)->a == NULL);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_or_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_or_short_circuit(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_or_true_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_or_true_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_or_true_3(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_pass_string_argument_to_game_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_say_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_say_call_count == 1);

	free(g);

	assert(streq(game_fn_say_message, "foo"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_pass_string_argument_to_helper_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_say_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_say_call_count == 1);

	free(g);

	assert(streq(game_fn_say_message, "foo"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_remainder_negative_result(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -1);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_remainder_positive_result(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 1);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_and_entity(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_b2_sprite_path, "tests/ok/resource_and_entity/foo.txt"));

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_and_entity/foo.txt"));

	assert(entities_size == 1);
	assert(streq(entities[0], "ok:foo"));
	assert(streq(entity_types[0], ""));
}

static void ok_resource_and_entity_and_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(streq(game_fn_define_a2_sprite_path, "tests/ok/resource_and_entity_and_on_fn/foo.txt"));

	((struct a2_on_fns *)on_fns)->a(g, 42);

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_and_entity_and_on_fn/foo.txt"));

	assert(entities_size == 1);
	assert(streq(entities[0], "ok:foo"));
	assert(streq(entity_types[0], ""));
}

static void ok_resource_and_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(streq(game_fn_define_w_sprite_path, "tests/ok/resource_and_on_fn/foo.txt"));

	((struct w_on_fns *)on_fns)->a(g, 42);

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_and_on_fn/foo.txt"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_u_sprite_path, "tests/ok/resource_can_contain_dot_1/.foo"));

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_1/.foo"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_u_sprite_path, "tests/ok/resource_can_contain_dot_2/foo."));

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_2/foo."));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_3(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_u_sprite_path, "tests/ok/resource_can_contain_dot_3/foo.bar"));

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_3/foo.bar"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_dot_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_u_sprite_path, "tests/ok/resource_can_contain_dot_dot_1/..foo"));

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_dot_1/..foo"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_dot_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_u_sprite_path, "tests/ok/resource_can_contain_dot_dot_2/foo.."));

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_dot_2/foo.."));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_dot_3(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_u_sprite_path, "tests/ok/resource_can_contain_dot_dot_3/foo..bar"));

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_dot_3/foo..bar"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_duplicate(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_v_foo, "tests/ok/resource_duplicate/foo.txt"));
	assert(streq(game_fn_define_v_bar, "tests/ok/resource_duplicate/foo.txt"));

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_duplicate/foo.txt"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_in_define(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_u_sprite_path, "tests/ok/resource_in_define/foo.txt"));

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_in_define/foo.txt"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_in_define_and_entity_in_game_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(streq(game_fn_define_w_sprite_path, "tests/ok/resource_in_define_and_entity_in_game_fn/foo.txt"));

	assert(game_fn_spawn_call_count == 0);
	((struct w_on_fns *)on_fns)->a(g, 42);
	assert(game_fn_spawn_call_count == 1);

	free(g);

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_in_define_and_entity_in_game_fn/foo.txt"));

	assert(entities_size == 1);
	assert(streq(entities[0], "ok:bar"));
	assert(streq(entity_types[0], ""));
}

static void ok_resource_twice(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(streq(game_fn_define_v_foo, "tests/ok/resource_twice/foo.txt"));
	assert(streq(game_fn_define_v_bar, "tests/ok/resource_twice/bar.txt"));

	free(g);

	assert(resources_size == 2);
	assert(streq(resources[0], "tests/ok/resource_twice/bar.txt"));
	assert(streq(resources[1], "tests/ok/resource_twice/foo.txt"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_return(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_return_from_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_return_with_no_value(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_stack_16_byte_alignment(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_aligned_call_count == 0);
	assert(game_fn_initialize_aligned_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_aligned_call_count == 1);
	assert(game_fn_initialize_aligned_call_count == 1);

	free(g);

	assert(game_fn_initialize_aligned_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_stack_16_byte_alignment_midway(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_magic_aligned_call_count == 0);
	assert(game_fn_initialize_aligned_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_magic_aligned_call_count == 1);
	assert(game_fn_initialize_aligned_call_count == 1);

	free(g);

	assert(game_fn_initialize_aligned_x == 42 + 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_string_and_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(streq(game_fn_define_p_x, "foo"));

	((struct p_on_fns *)on_fns)->a(g);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_string_eq_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_string_eq_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_string_eq_true_empty(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_string_ne_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_string_ne_false_empty(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_string_ne_true(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_subtraction_negative_result(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -3);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_subtraction_positive_result(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 3);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_does_not_shadow_define_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_reassignment(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 69);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_shadows_define_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_shadows_game_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_shadows_helper_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_shadows_on_fn_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_shadows_on_fn_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_string_global(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_say_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_say_call_count == 1);

	free(g);

	assert(streq(game_fn_say_message, "foo"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_string_local(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_say_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_say_call_count == 1);

	free(g);

	assert(streq(game_fn_say_message, "foo"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_void_function_early_return(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_while_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_write_to_global_variable(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_max_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_max_call_count == 1);

	free(g);

	assert(game_fn_max_x == 43);
	assert(game_fn_max_y == 69);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void error_tests(void) {
	TEST_ERROR(assign_to_unknown_variable);
	TEST_ERROR(assignment_isnt_expression);
	TEST_ERROR(bool_unary_minus);
	TEST_ERROR(cant_add_strings);
	TEST_ERROR(cant_call_define_fn_1);
	TEST_ERROR(cant_call_define_fn_2);
	TEST_ERROR(cant_redefine_global);
	TEST_ERROR(comment_at_the_end_of_another_statement);
	TEST_ERROR(comment_at_the_end_of_define);
	TEST_ERROR(comment_lone_global_at_end);
	TEST_ERROR(define_fn_calls_fn);
	TEST_ERROR(define_fn_different_name);
	TEST_ERROR(define_fn_not_enough_arguments);
	TEST_ERROR(define_fn_only_one_max);
	TEST_ERROR(define_fn_uses_global_variable);
	TEST_ERROR(define_fn_was_not_declared);
	TEST_ERROR(empty_line_after_group);
	TEST_ERROR(empty_line_at_start_of_file);
	TEST_ERROR(empty_line_before_group);
	TEST_ERROR(empty_line_fn_group);
	TEST_ERROR(empty_line_twice_at_end_of_file);
	TEST_ERROR(empty_line_twice_between_local_statements);
	TEST_ERROR(empty_line_while_group);
	TEST_ERROR(entity_cant_be_empty_string);
	TEST_ERROR(entity_has_invalid_entity_name_colon);
	TEST_ERROR(entity_has_invalid_entity_name_uppercase);
	TEST_ERROR(entity_has_invalid_mod_name_uppercase);
	TEST_ERROR(entity_mod_name_and_entity_name_is_missing);
	TEST_ERROR(entity_mod_name_cant_be_current_mod);
	TEST_ERROR(entity_mod_name_is_missing);
	TEST_ERROR(entity_name_is_missing);
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
	TEST_ERROR(global_variable_definition_missing_type);
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
	TEST_ERROR(indentation_going_down_by_2);
	TEST_ERROR(local_variable_already_exists);
	TEST_ERROR(local_variable_definition_cant_use_itself);
	TEST_ERROR(local_variable_definition_missing_type);
	TEST_ERROR(missing_define_fn);
	TEST_ERROR(missing_empty_line_between_define_fn_and_global);
	TEST_ERROR(missing_empty_line_between_global_and_on_fn);
	TEST_ERROR(missing_empty_line_between_on_fn_and_helper_fn);
	TEST_ERROR(no_space_between_comment_character_and_comment);
	TEST_ERROR(on_fn_before_define);
	TEST_ERROR(on_fn_cant_be_called_by_helper_fn);
	TEST_ERROR(on_fn_cant_be_called_by_on_fn);
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
	TEST_ERROR(resource_cant_be_empty_string);
	TEST_ERROR(resource_cant_contain_backslash);
	TEST_ERROR(resource_cant_contain_double_slash);
	TEST_ERROR(resource_cant_go_up_to_parent_directory_1);
	TEST_ERROR(resource_cant_go_up_to_parent_directory_2);
	TEST_ERROR(resource_cant_go_up_to_parent_directory_3);
	TEST_ERROR(resource_cant_go_up_to_parent_directory_4);
	TEST_ERROR(resource_cant_have_leading_slash);
	TEST_ERROR(resource_cant_have_trailing_slash);
	TEST_ERROR(resource_cant_refer_to_current_directory_1);
	TEST_ERROR(resource_cant_refer_to_current_directory_2);
	TEST_ERROR(resource_cant_refer_to_current_directory_3);
	TEST_ERROR(resource_cant_refer_to_current_directory_4);
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
	grug_set_runtime_error_handler(runtime_error_handler);

	TEST_RUNTIME_ERROR(division_by_0, "d", 0);
	TEST_RUNTIME_ERROR(stack_overflow, "d", 0);
	TEST_RUNTIME_ERROR(time_limit_exceeded, "d", 0);
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
	TEST_OK(comment_above_block, "d", 0);
	TEST_OK(comment_above_block_twice, "d", 0);
	TEST_OK(comment_above_define_fn, "d", 0);
	TEST_OK(comment_above_define_fn_twice, "d", 0);
	TEST_OK(comment_above_globals, "d", 12);
	TEST_OK(comment_above_helper_fn, "d", 0);
	TEST_OK(comment_above_on_fn, "d", 0);
	TEST_OK(comment_between_globals, "a", 8);
	TEST_OK(comment_between_statements, "d", 0);
	TEST_OK(comment_lone_block, "d", 0);
	TEST_OK(comment_lone_block_at_end, "d", 0);
	TEST_OK(comment_lone_global, "d", 0);
	TEST_OK(continue, "d", 0);
	TEST_OK(define, "h", 0);
	TEST_OK(define_containing_addition, "b", 0);
	TEST_OK(define_containing_string, "k", 0);
	TEST_OK(define_with_eight_f32_fields, "t", 0);
	TEST_OK(define_with_six_fields, "m", 0);
	TEST_OK(define_with_six_i32_fields, "n", 0);
	TEST_OK(define_with_six_string_fields, "o", 0);
	TEST_OK(division_negative_result, "d", 0);
	TEST_OK(division_positive_result, "d", 0);
	TEST_OK(else_after_else_if_false, "d", 0);
	TEST_OK(else_after_else_if_true, "d", 0);
	TEST_OK(else_false, "d", 0);
	TEST_OK(else_if_false, "d", 0);
	TEST_OK(else_if_true, "d", 0);
	TEST_OK(else_true, "d", 0);
	TEST_OK(empty_line, "d", 0);
	TEST_OK(entity_and_on_fn, "z", 0);
	TEST_OK(entity_and_resource_as_subexpression, "d", 0);
	TEST_OK(entity_duplicate, "y", 0);
	TEST_OK(entity_in_define, "x", 0);
	TEST_OK(entity_in_define_with_mod_specified, "x", 0);
	TEST_OK(entity_twice, "y", 0);
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
	TEST_OK(minimal, "a", 0);
	TEST_OK(multiplication_as_two_arguments, "d", 0);
	TEST_OK(ne_false, "d", 0);
	TEST_OK(negate_parenthesized_expr, "d", 0);
	TEST_OK(negative_literal, "d", 0);
	TEST_OK(nested_break, "d", 0);
	TEST_OK(nested_continue, "d", 0);
	TEST_OK(ne_true, "d", 0);
	TEST_OK(no_define_fields, "d", 0);
	TEST_OK(no_on_fns, "a", 0);
	TEST_OK(no_empty_line_between_globals, "a", 8);
	TEST_OK(no_empty_line_between_statements, "d", 0);
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
	TEST_OK(on_fn_usage_is_optional, "d", 0);
	TEST_OK(or_false, "d", 0);
	TEST_OK(or_short_circuit, "d", 0);
	TEST_OK(or_true_1, "d", 0);
	TEST_OK(or_true_2, "d", 0);
	TEST_OK(or_true_3, "d", 0);
	TEST_OK(pass_string_argument_to_game_fn, "d", 0);
	TEST_OK(pass_string_argument_to_helper_fn, "d", 0);
	TEST_OK(remainder_negative_result, "d", 0);
	TEST_OK(remainder_positive_result, "d", 0);
	TEST_OK(resource_and_entity, "b2", 0);
	TEST_OK(resource_and_entity_and_on_fn, "a2", 0);
	TEST_OK(resource_and_on_fn, "w", 0);
	TEST_OK(resource_can_contain_dot_1, "u", 0);
	TEST_OK(resource_can_contain_dot_2, "u", 0);
	TEST_OK(resource_can_contain_dot_3, "u", 0);
	TEST_OK(resource_can_contain_dot_dot_1, "u", 0);
	TEST_OK(resource_can_contain_dot_dot_2, "u", 0);
	TEST_OK(resource_can_contain_dot_dot_3, "u", 0);
	TEST_OK(resource_duplicate, "v", 0);
	TEST_OK(resource_in_define, "u", 0);
	TEST_OK(resource_in_define_and_entity_in_game_fn, "w", 0);
	TEST_OK(resource_twice, "v", 0);
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

int main(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		whitelisted_tests[whitelisted_tests_size++] = argv[i];
	}

	error_tests();
	runtime_error_tests();
	ok_tests();

	printf("\nAll tests passed! 🎉\n");
}
