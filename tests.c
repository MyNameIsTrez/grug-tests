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
#include <time.h>
#include <unistd.h>

// Forward declaration, since grug.h doesn't declare it
bool grug_test_regenerate_dll(char *grug_file_path, char *dll_path, char *mod);

// From https://stackoverflow.com/a/2114249/13279557
#ifdef __x86_64__
#define ASSERT_16_BYTE_STACK_ALIGNED() {\
	int64_t rsp;\
	\
	_Pragma("GCC diagnostic push")\
	_Pragma("GCC diagnostic ignored \"-Wlanguage-extension-token\"")\
	\
	__asm__ volatile("mov %%rsp, %0\n\t" : "=r" (rsp));\
	\
	_Pragma("GCC diagnostic pop")\
	\
	if ((rsp & 0xf) != 0) {\
		static char msg[] = "The stack was not 16-byte aligned!\n";\
		write(STDERR_FILENO, msg, sizeof(msg) - 1);\
		abort();\
	}\
}
#elif __aarch64__
#define ASSERT_16_BYTE_STACK_ALIGNED() {\
	int64_t rsp;\
	\
	_Pragma("GCC diagnostic push")\
	_Pragma("GCC diagnostic ignored \"-Wlanguage-extension-token\"")\
	\
	__asm__ volatile("mov %0, sp\n\t" : "=r" (rsp));\
	\
	_Pragma("GCC diagnostic pop")\
	\
	if ((rsp & 0xf) != 0) {\
		static char msg[] = "The stack was not 16-byte aligned!\n";\
		write(STDERR_FILENO, msg, sizeof(msg) - 1);\
		abort();\
	}\
}
#else
#error Unsupported or unrecognized architecture
#endif

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

struct error_test_data {
	char *test_name_str;
	char *grug_path;
	char *expected_error_path;
	char *results_path;
	char *output_dll_path;
	char *grug_output_path;
	char *failed_file_path;
};
static struct error_test_data error_test_datas[420420];
static size_t error_test_datas_size;

struct runtime_error_test_data {
	void (*run)(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types);
	char *test_name_str;
	char *grug_path;
	char *nasm_path;
	char *expected_error_path;
	char *results_path;
	char *output_dll_path;
	char *expected_dll_path;
	char *nasm_o_path;
	char *output_xxd_path;
	char *expected_xxd_path;
	char *output_readelf_path;
	char *expected_readelf_path;
	char *output_objdump_path;
	char *expected_objdump_path;
	char *dump_path;
	char *applied_path;
	char *failed_file_path;
	char *expected_define_type_str;
	size_t expected_globals_size_value;
};
static struct runtime_error_test_data runtime_error_test_datas[420420];
static size_t runtime_error_test_datas_size;

struct ok_test_data {
	void (*run)(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types);
	char *test_name_str;
	char *grug_path;
	char *nasm_path;
	char *results_path;
	char *output_dll_path;
	char *expected_dll_path;
	char *nasm_o_path;
	char *output_xxd_path;
	char *expected_xxd_path;
	char *output_readelf_path;
	char *expected_readelf_path;
	char *output_objdump_path;
	char *expected_objdump_path;
	char *dump_path;
	char *applied_path;
	char *failed_file_path;
	char *expected_define_type_str;
	size_t expected_globals_size_value;
};
static struct ok_test_data ok_test_datas[420420];
static size_t ok_test_datas_size;

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
static size_t game_fn_get_opponent_call_count;
static size_t game_fn_set_target_call_count;
static size_t game_fn_motherload_call_count;
static size_t game_fn_motherload_subless_call_count;
static size_t game_fn_offset_32_bit_f32_call_count;
static size_t game_fn_offset_32_bit_i32_call_count;
static size_t game_fn_offset_32_bit_string_call_count;

static bool streq(char *a, char *b) {
	return strcmp(a, b) == 0;
}

void game_fn_nothing(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_nothing_call_count++;
}
int32_t game_fn_magic(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_magic_call_count++;

	return 42;
}
static int32_t game_fn_initialize_x;
void game_fn_initialize(int32_t x) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_initialize_call_count++;

	game_fn_initialize_x = x;
}
static bool game_fn_initialize_bool_b;
void game_fn_initialize_bool(bool b) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_initialize_bool_call_count++;

	game_fn_initialize_bool_b = b;
}
static int32_t game_fn_identity_x;
int32_t game_fn_identity(int32_t x) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_identity_call_count++;

	game_fn_identity_x = x;

	return x;
}
static int32_t game_fn_max_x;
static int32_t game_fn_max_y;
int32_t game_fn_max(int32_t x, int32_t y) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_max_call_count++;

	game_fn_max_x = x;
	game_fn_max_y = y;

	return x > y ? x : y;
}
static char *game_fn_say_message;
void game_fn_say(char *message) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_say_call_count++;

	game_fn_say_message = message;
}
static float game_fn_sin_x;
float game_fn_sin(float x) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_sin_call_count++;

	game_fn_sin_x = x;

	return sinf(x);
}
static float game_fn_cos_x;
float game_fn_cos(float x) {
	ASSERT_16_BYTE_STACK_ALIGNED();
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
static int32_t game_fn_mega_id;
static char *game_fn_mega_str;
void game_fn_mega(float f1, int32_t i1, bool b1, float f2, float f3, float f4, bool b2, int32_t i2, float f5, float f6, float f7, float f8, uint64_t id, char *str) {
	ASSERT_16_BYTE_STACK_ALIGNED();
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
	game_fn_mega_id = id;
	game_fn_mega_str = str;
}
bool game_fn_is_friday(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_is_friday_call_count++;

	return true;
}
static bool game_fn_set_is_happy_is_happy;
void game_fn_set_is_happy(bool is_happy) {
	ASSERT_16_BYTE_STACK_ALIGNED();
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
	ASSERT_16_BYTE_STACK_ALIGNED();
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
	ASSERT_16_BYTE_STACK_ALIGNED();
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
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_blocked_alrm_call_count++;
}
void game_fn_nothing_aligned(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_nothing_aligned_call_count++;
}
int32_t game_fn_magic_aligned(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_magic_aligned_call_count++;
	return 42;
}
static int32_t game_fn_initialize_aligned_x;
void game_fn_initialize_aligned(int32_t x) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_initialize_aligned_call_count++;
	game_fn_initialize_aligned_x = x;
}
static char *game_fn_spawn_name;
void game_fn_spawn(char *name) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_spawn_call_count++;

	game_fn_spawn_name = name;
}
static char *game_fn_has_resource_path;
bool game_fn_has_resource(char *path) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_has_resource_call_count++;

	game_fn_has_resource_path = path;

	return true;
}
static char *game_fn_has_entity_name;
bool game_fn_has_entity(char *name) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_has_entity_call_count++;

	game_fn_has_entity_name = name;

	return true;
}
static char *game_fn_has_string_str;
bool game_fn_has_string(char *str) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_has_string_call_count++;

	game_fn_has_string_str = str;

	return true;
}
uint64_t game_fn_get_opponent(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_get_opponent_call_count++;

	return 42;
}
static uint64_t game_fn_set_target_target;
void game_fn_set_target(uint64_t target) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_set_target_call_count++;

	game_fn_set_target_target = target;
}
static int32_t game_fn_motherload_i1;
static int32_t game_fn_motherload_i2;
static int32_t game_fn_motherload_i3;
static int32_t game_fn_motherload_i4;
static int32_t game_fn_motherload_i5;
static int32_t game_fn_motherload_i6;
static int32_t game_fn_motherload_i7;
static float game_fn_motherload_f1;
static float game_fn_motherload_f2;
static float game_fn_motherload_f3;
static float game_fn_motherload_f4;
static float game_fn_motherload_f5;
static float game_fn_motherload_f6;
static float game_fn_motherload_f7;
static float game_fn_motherload_f8;
static uint64_t game_fn_motherload_id;
static float game_fn_motherload_f9;
void game_fn_motherload(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, uint64_t id, float f9) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_motherload_call_count++;

	game_fn_motherload_i1 = i1;
	game_fn_motherload_i2 = i2;
	game_fn_motherload_i3 = i3;
	game_fn_motherload_i4 = i4;
	game_fn_motherload_i5 = i5;
	game_fn_motherload_i6 = i6;
	game_fn_motherload_i7 = i7;
	game_fn_motherload_f1 = f1;
	game_fn_motherload_f2 = f2;
	game_fn_motherload_f3 = f3;
	game_fn_motherload_f4 = f4;
	game_fn_motherload_f5 = f5;
	game_fn_motherload_f6 = f6;
	game_fn_motherload_f7 = f7;
	game_fn_motherload_f8 = f8;
	game_fn_motherload_id = id;
	game_fn_motherload_f9 = f9;
}
static int32_t game_fn_motherload_subless_i1;
static int32_t game_fn_motherload_subless_i2;
static int32_t game_fn_motherload_subless_i3;
static int32_t game_fn_motherload_subless_i4;
static int32_t game_fn_motherload_subless_i5;
static int32_t game_fn_motherload_subless_i6;
static int32_t game_fn_motherload_subless_i7;
static float game_fn_motherload_subless_f1;
static float game_fn_motherload_subless_f2;
static float game_fn_motherload_subless_f3;
static float game_fn_motherload_subless_f4;
static float game_fn_motherload_subless_f5;
static float game_fn_motherload_subless_f6;
static float game_fn_motherload_subless_f7;
static float game_fn_motherload_subless_f8;
static float game_fn_motherload_subless_f9;
static uint64_t game_fn_motherload_subless_id;
static float game_fn_motherload_subless_f10;
void game_fn_motherload_subless(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, uint64_t id, float f10) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_motherload_subless_call_count++;

	game_fn_motherload_subless_i1 = i1;
	game_fn_motherload_subless_i2 = i2;
	game_fn_motherload_subless_i3 = i3;
	game_fn_motherload_subless_i4 = i4;
	game_fn_motherload_subless_i5 = i5;
	game_fn_motherload_subless_i6 = i6;
	game_fn_motherload_subless_i7 = i7;
	game_fn_motherload_subless_f1 = f1;
	game_fn_motherload_subless_f2 = f2;
	game_fn_motherload_subless_f3 = f3;
	game_fn_motherload_subless_f4 = f4;
	game_fn_motherload_subless_f5 = f5;
	game_fn_motherload_subless_f6 = f6;
	game_fn_motherload_subless_f7 = f7;
	game_fn_motherload_subless_f8 = f8;
	game_fn_motherload_subless_f9 = f9;
	game_fn_motherload_subless_id = id;
	game_fn_motherload_subless_f10 = f10;
}
static char *game_fn_offset_32_bit_f32_s1;
static char *game_fn_offset_32_bit_f32_s2;
static char *game_fn_offset_32_bit_f32_s3;
static char *game_fn_offset_32_bit_f32_s4;
static char *game_fn_offset_32_bit_f32_s5;
static char *game_fn_offset_32_bit_f32_s6;
static char *game_fn_offset_32_bit_f32_s7;
static char *game_fn_offset_32_bit_f32_s8;
static char *game_fn_offset_32_bit_f32_s9;
static char *game_fn_offset_32_bit_f32_s10;
static char *game_fn_offset_32_bit_f32_s11;
static char *game_fn_offset_32_bit_f32_s12;
static char *game_fn_offset_32_bit_f32_s13;
static char *game_fn_offset_32_bit_f32_s14;
static char *game_fn_offset_32_bit_f32_s15;
static float game_fn_offset_32_bit_f32_f1;
static float game_fn_offset_32_bit_f32_f2;
static float game_fn_offset_32_bit_f32_f3;
static float game_fn_offset_32_bit_f32_f4;
static float game_fn_offset_32_bit_f32_f5;
static float game_fn_offset_32_bit_f32_f6;
static float game_fn_offset_32_bit_f32_f7;
static float game_fn_offset_32_bit_f32_f8;
static int32_t game_fn_offset_32_bit_f32_g;
void game_fn_offset_32_bit_f32(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7, char *s8, char *s9, char *s10, char *s11, char *s12, char *s13, char *s14, char *s15, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, int32_t g) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_offset_32_bit_f32_call_count++;

	game_fn_offset_32_bit_f32_s1 = s1;
	game_fn_offset_32_bit_f32_s2 = s2;
	game_fn_offset_32_bit_f32_s3 = s3;
	game_fn_offset_32_bit_f32_s4 = s4;
	game_fn_offset_32_bit_f32_s5 = s5;
	game_fn_offset_32_bit_f32_s6 = s6;
	game_fn_offset_32_bit_f32_s7 = s7;
	game_fn_offset_32_bit_f32_s8 = s8;
	game_fn_offset_32_bit_f32_s9 = s9;
	game_fn_offset_32_bit_f32_s10 = s10;
	game_fn_offset_32_bit_f32_s11 = s11;
	game_fn_offset_32_bit_f32_s12 = s12;
	game_fn_offset_32_bit_f32_s13 = s13;
	game_fn_offset_32_bit_f32_s14 = s14;
	game_fn_offset_32_bit_f32_s15 = s15;
	game_fn_offset_32_bit_f32_f1 = f1;
	game_fn_offset_32_bit_f32_f2 = f2;
	game_fn_offset_32_bit_f32_f3 = f3;
	game_fn_offset_32_bit_f32_f4 = f4;
	game_fn_offset_32_bit_f32_f5 = f5;
	game_fn_offset_32_bit_f32_f6 = f6;
	game_fn_offset_32_bit_f32_f7 = f7;
	game_fn_offset_32_bit_f32_f8 = f8;
	game_fn_offset_32_bit_f32_g = g;
}
static float game_fn_offset_32_bit_i32_f1;
static float game_fn_offset_32_bit_i32_f2;
static float game_fn_offset_32_bit_i32_f3;
static float game_fn_offset_32_bit_i32_f4;
static float game_fn_offset_32_bit_i32_f5;
static float game_fn_offset_32_bit_i32_f6;
static float game_fn_offset_32_bit_i32_f7;
static float game_fn_offset_32_bit_i32_f8;
static float game_fn_offset_32_bit_i32_f9;
static float game_fn_offset_32_bit_i32_f10;
static float game_fn_offset_32_bit_i32_f11;
static float game_fn_offset_32_bit_i32_f12;
static float game_fn_offset_32_bit_i32_f13;
static float game_fn_offset_32_bit_i32_f14;
static float game_fn_offset_32_bit_i32_f15;
static float game_fn_offset_32_bit_i32_f16;
static float game_fn_offset_32_bit_i32_f17;
static float game_fn_offset_32_bit_i32_f18;
static float game_fn_offset_32_bit_i32_f19;
static float game_fn_offset_32_bit_i32_f20;
static float game_fn_offset_32_bit_i32_f21;
static float game_fn_offset_32_bit_i32_f22;
static float game_fn_offset_32_bit_i32_f23;
static float game_fn_offset_32_bit_i32_f24;
static float game_fn_offset_32_bit_i32_f25;
static float game_fn_offset_32_bit_i32_f26;
static float game_fn_offset_32_bit_i32_f27;
static float game_fn_offset_32_bit_i32_f28;
static float game_fn_offset_32_bit_i32_f29;
static float game_fn_offset_32_bit_i32_f30;
static int32_t game_fn_offset_32_bit_i32_i1;
static int32_t game_fn_offset_32_bit_i32_i2;
static int32_t game_fn_offset_32_bit_i32_i3;
static int32_t game_fn_offset_32_bit_i32_i4;
static int32_t game_fn_offset_32_bit_i32_i5;
static int32_t game_fn_offset_32_bit_i32_g;
void game_fn_offset_32_bit_i32(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t g) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_offset_32_bit_i32_call_count++;

	game_fn_offset_32_bit_i32_f1 = f1;
	game_fn_offset_32_bit_i32_f2 = f2;
	game_fn_offset_32_bit_i32_f3 = f3;
	game_fn_offset_32_bit_i32_f4 = f4;
	game_fn_offset_32_bit_i32_f5 = f5;
	game_fn_offset_32_bit_i32_f6 = f6;
	game_fn_offset_32_bit_i32_f7 = f7;
	game_fn_offset_32_bit_i32_f8 = f8;
	game_fn_offset_32_bit_i32_f9 = f9;
	game_fn_offset_32_bit_i32_f10 = f10;
	game_fn_offset_32_bit_i32_f11 = f11;
	game_fn_offset_32_bit_i32_f12 = f12;
	game_fn_offset_32_bit_i32_f13 = f13;
	game_fn_offset_32_bit_i32_f14 = f14;
	game_fn_offset_32_bit_i32_f15 = f15;
	game_fn_offset_32_bit_i32_f16 = f16;
	game_fn_offset_32_bit_i32_f17 = f17;
	game_fn_offset_32_bit_i32_f18 = f18;
	game_fn_offset_32_bit_i32_f19 = f19;
	game_fn_offset_32_bit_i32_f20 = f20;
	game_fn_offset_32_bit_i32_f21 = f21;
	game_fn_offset_32_bit_i32_f22 = f22;
	game_fn_offset_32_bit_i32_f23 = f23;
	game_fn_offset_32_bit_i32_f24 = f24;
	game_fn_offset_32_bit_i32_f25 = f25;
	game_fn_offset_32_bit_i32_f26 = f26;
	game_fn_offset_32_bit_i32_f27 = f27;
	game_fn_offset_32_bit_i32_f28 = f28;
	game_fn_offset_32_bit_i32_f29 = f29;
	game_fn_offset_32_bit_i32_f30 = f30;
	game_fn_offset_32_bit_i32_i1 = i1;
	game_fn_offset_32_bit_i32_i2 = i2;
	game_fn_offset_32_bit_i32_i3 = i3;
	game_fn_offset_32_bit_i32_i4 = i4;
	game_fn_offset_32_bit_i32_i5 = i5;
	game_fn_offset_32_bit_i32_g = g;
}
static float game_fn_offset_32_bit_string_f1;
static float game_fn_offset_32_bit_string_f2;
static float game_fn_offset_32_bit_string_f3;
static float game_fn_offset_32_bit_string_f4;
static float game_fn_offset_32_bit_string_f5;
static float game_fn_offset_32_bit_string_f6;
static float game_fn_offset_32_bit_string_f7;
static float game_fn_offset_32_bit_string_f8;
static float game_fn_offset_32_bit_string_f9;
static float game_fn_offset_32_bit_string_f10;
static float game_fn_offset_32_bit_string_f11;
static float game_fn_offset_32_bit_string_f12;
static float game_fn_offset_32_bit_string_f13;
static float game_fn_offset_32_bit_string_f14;
static float game_fn_offset_32_bit_string_f15;
static float game_fn_offset_32_bit_string_f16;
static float game_fn_offset_32_bit_string_f17;
static float game_fn_offset_32_bit_string_f18;
static float game_fn_offset_32_bit_string_f19;
static float game_fn_offset_32_bit_string_f20;
static float game_fn_offset_32_bit_string_f21;
static float game_fn_offset_32_bit_string_f22;
static float game_fn_offset_32_bit_string_f23;
static float game_fn_offset_32_bit_string_f24;
static float game_fn_offset_32_bit_string_f25;
static float game_fn_offset_32_bit_string_f26;
static float game_fn_offset_32_bit_string_f27;
static float game_fn_offset_32_bit_string_f28;
static float game_fn_offset_32_bit_string_f29;
static float game_fn_offset_32_bit_string_f30;
static char *game_fn_offset_32_bit_string_s1;
static char *game_fn_offset_32_bit_string_s2;
static char *game_fn_offset_32_bit_string_s3;
static char *game_fn_offset_32_bit_string_s4;
static char *game_fn_offset_32_bit_string_s5;
static int32_t game_fn_offset_32_bit_string_g;
void game_fn_offset_32_bit_string(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, char *s1, char *s2, char *s3, char *s4, char *s5, int32_t g) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_offset_32_bit_string_call_count++;

	game_fn_offset_32_bit_string_f1 = f1;
	game_fn_offset_32_bit_string_f2 = f2;
	game_fn_offset_32_bit_string_f3 = f3;
	game_fn_offset_32_bit_string_f4 = f4;
	game_fn_offset_32_bit_string_f5 = f5;
	game_fn_offset_32_bit_string_f6 = f6;
	game_fn_offset_32_bit_string_f7 = f7;
	game_fn_offset_32_bit_string_f8 = f8;
	game_fn_offset_32_bit_string_f9 = f9;
	game_fn_offset_32_bit_string_f10 = f10;
	game_fn_offset_32_bit_string_f11 = f11;
	game_fn_offset_32_bit_string_f12 = f12;
	game_fn_offset_32_bit_string_f13 = f13;
	game_fn_offset_32_bit_string_f14 = f14;
	game_fn_offset_32_bit_string_f15 = f15;
	game_fn_offset_32_bit_string_f16 = f16;
	game_fn_offset_32_bit_string_f17 = f17;
	game_fn_offset_32_bit_string_f18 = f18;
	game_fn_offset_32_bit_string_f19 = f19;
	game_fn_offset_32_bit_string_f20 = f20;
	game_fn_offset_32_bit_string_f21 = f21;
	game_fn_offset_32_bit_string_f22 = f22;
	game_fn_offset_32_bit_string_f23 = f23;
	game_fn_offset_32_bit_string_f24 = f24;
	game_fn_offset_32_bit_string_f25 = f25;
	game_fn_offset_32_bit_string_f26 = f26;
	game_fn_offset_32_bit_string_f27 = f27;
	game_fn_offset_32_bit_string_f28 = f28;
	game_fn_offset_32_bit_string_f29 = f29;
	game_fn_offset_32_bit_string_f30 = f30;
	game_fn_offset_32_bit_string_s1 = s1;
	game_fn_offset_32_bit_string_s2 = s2;
	game_fn_offset_32_bit_string_s3 = s3;
	game_fn_offset_32_bit_string_s4 = s4;
	game_fn_offset_32_bit_string_s5 = s5;
	game_fn_offset_32_bit_string_g = g;
}

void game_fn_define_a(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
}
static int32_t game_fn_define_b_x;
void game_fn_define_b(int32_t x) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_b_x = x;
}
static int32_t game_fn_define_c_x;
static int32_t game_fn_define_c_y;
void game_fn_define_c(int32_t x, int32_t y) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_c_x = x;
	game_fn_define_c_y = y;
}
void game_fn_define_d(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
}
void game_fn_define_e(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
}
void game_fn_define_f(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
}
void game_fn_define_g(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
}
static int32_t game_fn_define_h_x;
void game_fn_define_h(int32_t x) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_h_x = x;
}
static int32_t game_fn_define_i_x;
static int32_t game_fn_define_i_y;
void game_fn_define_i(int32_t x, int32_t y) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_i_x = x;
	game_fn_define_i_y = y;
}
void game_fn_define_j(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
}
static int32_t game_fn_define_k_age;
static char *game_fn_define_k_name;
void game_fn_define_k(int32_t age, char *name) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_k_age = age;
	game_fn_define_k_name = name;
}
static char *game_fn_define_l_group;
static char *game_fn_define_l_name;
void game_fn_define_l(char *group, char *name) {
	ASSERT_16_BYTE_STACK_ALIGNED();
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
	ASSERT_16_BYTE_STACK_ALIGNED();
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
	ASSERT_16_BYTE_STACK_ALIGNED();
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
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_o_u = u;
	game_fn_define_o_v = v;
	game_fn_define_o_w = w;
	game_fn_define_o_x = x;
	game_fn_define_o_y = y;
	game_fn_define_o_z = z;
}
static char *game_fn_define_p_x;
void game_fn_define_p(char *x) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_p_x = x;
}
static char *game_fn_define_q_a;
static char *game_fn_define_q_b;
static char *game_fn_define_q_c;
void game_fn_define_q(char *a, char *b, char *c) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_q_a = a;
	game_fn_define_q_b = b;
	game_fn_define_q_c = c;
}
void game_fn_define_r(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
}
void game_fn_define_s(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
}
static float game_fn_define_t_f1;
static float game_fn_define_t_f2;
static float game_fn_define_t_f3;
static float game_fn_define_t_f4;
static float game_fn_define_t_f5;
static float game_fn_define_t_f6;
static float game_fn_define_t_f7;
static float game_fn_define_t_f8;
void game_fn_define_t(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8) {
	ASSERT_16_BYTE_STACK_ALIGNED();
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
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_u_sprite_path = sprite_path;
}
static char *game_fn_define_v_foo;
static char *game_fn_define_v_bar;
void game_fn_define_v(char *foo, char *bar) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_v_foo = foo;
	game_fn_define_v_bar = bar;
}
static char *game_fn_define_w_sprite_path;
void game_fn_define_w(char *sprite_path) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_w_sprite_path = sprite_path;
}
static char *game_fn_define_x_projectile;
void game_fn_define_x(char *projectile) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_x_projectile = projectile;
}
static char *game_fn_define_y_foo;
static char *game_fn_define_y_bar;
void game_fn_define_y(char *foo, char *bar) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_y_foo = foo;
	game_fn_define_y_bar = bar;
}
static char *game_fn_define_z_projectile;
void game_fn_define_z(char *projectile) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_z_projectile = projectile;
}
static char *game_fn_define_a2_sprite_path;
static char *game_fn_define_a2_projectile;
void game_fn_define_a2(char *sprite_path, char *projectile) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_a2_sprite_path = sprite_path;
	game_fn_define_a2_projectile = projectile;
}
static char *game_fn_define_b2_sprite_path;
static char *game_fn_define_b2_projectile;
void game_fn_define_b2(char *sprite_path, char *projectile) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_b2_sprite_path = sprite_path;
	game_fn_define_b2_projectile = projectile;
}
static int32_t game_fn_define_c2_i1;
static int32_t game_fn_define_c2_i2;
static int32_t game_fn_define_c2_i3;
static int32_t game_fn_define_c2_i4;
static int32_t game_fn_define_c2_i5;
static int32_t game_fn_define_c2_i6;
static int32_t game_fn_define_c2_i7;
static float game_fn_define_c2_f1;
static float game_fn_define_c2_f2;
static float game_fn_define_c2_f3;
static float game_fn_define_c2_f4;
static float game_fn_define_c2_f5;
static float game_fn_define_c2_f6;
static float game_fn_define_c2_f7;
static float game_fn_define_c2_f8;
static float game_fn_define_c2_f9;
static int32_t game_fn_define_c2_i8;
static float game_fn_define_c2_f10;
void game_fn_define_c2(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, int32_t i8, float f10) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_c2_i1 = i1;
	game_fn_define_c2_i2 = i2;
	game_fn_define_c2_i3 = i3;
	game_fn_define_c2_i4 = i4;
	game_fn_define_c2_i5 = i5;
	game_fn_define_c2_i6 = i6;
	game_fn_define_c2_i7 = i7;
	game_fn_define_c2_f1 = f1;
	game_fn_define_c2_f2 = f2;
	game_fn_define_c2_f3 = f3;
	game_fn_define_c2_f4 = f4;
	game_fn_define_c2_f5 = f5;
	game_fn_define_c2_f6 = f6;
	game_fn_define_c2_f7 = f7;
	game_fn_define_c2_f8 = f8;
	game_fn_define_c2_f9 = f9;
	game_fn_define_c2_i8 = i8;
	game_fn_define_c2_f10 = f10;
}
static int32_t game_fn_define_d2_i1;
static int32_t game_fn_define_d2_i2;
static int32_t game_fn_define_d2_i3;
static int32_t game_fn_define_d2_i4;
static int32_t game_fn_define_d2_i5;
static int32_t game_fn_define_d2_i6;
static int32_t game_fn_define_d2_i7;
static float game_fn_define_d2_f1;
static float game_fn_define_d2_f2;
static float game_fn_define_d2_f3;
static float game_fn_define_d2_f4;
static float game_fn_define_d2_f5;
static float game_fn_define_d2_f6;
static float game_fn_define_d2_f7;
static float game_fn_define_d2_f8;
static int32_t game_fn_define_d2_i8;
static float game_fn_define_d2_f9;
void game_fn_define_d2(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, int32_t i8, float f9) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_define_d2_i1 = i1;
	game_fn_define_d2_i2 = i2;
	game_fn_define_d2_i3 = i3;
	game_fn_define_d2_i4 = i4;
	game_fn_define_d2_i5 = i5;
	game_fn_define_d2_i6 = i6;
	game_fn_define_d2_i7 = i7;
	game_fn_define_d2_f1 = f1;
	game_fn_define_d2_f2 = f2;
	game_fn_define_d2_f3 = f3;
	game_fn_define_d2_f4 = f4;
	game_fn_define_d2_f5 = f5;
	game_fn_define_d2_f6 = f6;
	game_fn_define_d2_f7 = f7;
	game_fn_define_d2_f8 = f8;
	game_fn_define_d2_i8 = i8;
	game_fn_define_d2_f9 = f9;
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
	game_fn_get_opponent_call_count = 0;
	game_fn_set_target_call_count = 0;
	game_fn_motherload_call_count = 0;
	game_fn_motherload_subless_call_count = 0;
	game_fn_offset_32_bit_f32_call_count = 0;
	game_fn_offset_32_bit_i32_call_count = 0;
	game_fn_offset_32_bit_string_call_count = 0;
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

#define ADD_TEST_ERROR(test_name) {\
	if (whitelisted_tests_size == 0 || is_whitelisted_test(#test_name)) {\
		error_test_datas[error_test_datas_size++] = (struct error_test_data){\
			.test_name_str = #test_name,\
			.grug_path = "tests/err/"#test_name"/input.grug",\
			.expected_error_path = "tests/err/"#test_name"/expected_error.txt",\
			.results_path = "tests/err/"#test_name"/results",\
			.output_dll_path = "tests/err/"#test_name"/results/output.so",\
			.grug_output_path = "tests/err/"#test_name"/results/grug_output.txt",\
			.failed_file_path = "tests/err/"#test_name"/results/failed"\
		};\
	}\
}

#define ADD_TEST_RUNTIME_ERROR(test_name, expected_define_type, expected_globals_size) {\
	if (whitelisted_tests_size == 0 || is_whitelisted_test(#test_name)) {\
		runtime_error_test_datas[runtime_error_test_datas_size++] = (struct runtime_error_test_data){\
			.run = runtime_error_##test_name,\
			.test_name_str = #test_name,\
			.grug_path = "tests/err_runtime/"#test_name"/input.grug",\
			.nasm_path = "tests/err_runtime/"#test_name"/input.s",\
			.expected_error_path = "tests/err_runtime/"#test_name"/expected_error.txt",\
			.results_path = "tests/err_runtime/"#test_name"/results",\
			.output_dll_path = "tests/err_runtime/"#test_name"/results/output.so",\
			.expected_dll_path = "tests/err_runtime/"#test_name"/results/expected.so",\
			.nasm_o_path = "tests/err_runtime/"#test_name"/results/expected.o",\
			.output_xxd_path = "tests/err_runtime/"#test_name"/results/output.hex",\
			.expected_xxd_path = "tests/err_runtime/"#test_name"/results/expected.hex",\
			.output_readelf_path = "tests/err_runtime/"#test_name"/results/output_elf.log",\
			.expected_readelf_path = "tests/err_runtime/"#test_name"/results/expected_elf.log",\
			.output_objdump_path = "tests/err_runtime/"#test_name"/results/output_objdump.log",\
			.expected_objdump_path = "tests/err_runtime/"#test_name"/results/expected_objdump.log",\
			.dump_path = "tests/err_runtime/"#test_name"/results/dump.json",\
			.applied_path = "tests/err_runtime/"#test_name"/results/applied.grug",\
			.failed_file_path = "tests/err_runtime/"#test_name"/results/failed",\
			.expected_define_type_str = expected_define_type,\
			.expected_globals_size_value = expected_globals_size\
		};\
	}\
}

#define ADD_TEST_OK(test_name, expected_define_type, expected_globals_size) {\
	if (whitelisted_tests_size == 0 || is_whitelisted_test(#test_name)) {\
		ok_test_datas[ok_test_datas_size++] = (struct ok_test_data){\
			.run = ok_##test_name,\
			.test_name_str = #test_name,\
			.grug_path = "tests/ok/"#test_name"/input.grug",\
			.nasm_path = "tests/ok/"#test_name"/input.s",\
			.results_path = "tests/ok/"#test_name"/results",\
			.output_dll_path = "tests/ok/"#test_name"/results/output.so",\
			.expected_dll_path = "tests/ok/"#test_name"/results/expected.so",\
			.nasm_o_path = "tests/ok/"#test_name"/results/expected.o",\
			.output_xxd_path = "tests/ok/"#test_name"/results/output.hex",\
			.expected_xxd_path = "tests/ok/"#test_name"/results/expected.hex",\
			.output_readelf_path = "tests/ok/"#test_name"/results/output_elf.log",\
			.expected_readelf_path = "tests/ok/"#test_name"/results/expected_elf.log",\
			.output_objdump_path = "tests/ok/"#test_name"/results/output_objdump.log",\
			.expected_objdump_path = "tests/ok/"#test_name"/results/expected_objdump.log",\
			.dump_path = "tests/ok/"#test_name"/results/dump.json",\
			.applied_path = "tests/ok/"#test_name"/results/applied.grug",\
			.failed_file_path = "tests/ok/"#test_name"/results/failed",\
			.expected_define_type_str = expected_define_type,\
			.expected_globals_size_value = expected_globals_size\
		};\
	}\
}

// This is the Fisher-Yates shuffle:
// https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
// https://blog.codinghorror.com/the-danger-of-naivete/
#define SHUFFLE(arr, size, T) {\
	for (int i = size; i > 0; i--) {\
		int n = rand() % i;\
		\
		T old = arr[i-1];\
		arr[i-1] = arr[n];\
		arr[n] = old;\
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

#ifdef OUTPUT_DLL_INFO
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

	run_and_write((char *[]){"objdump", "-d", dll_path, "-Mintel", NULL}, objdump_path);
}
#endif

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
	static char expected_error[420420];
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

	(void)output_xxd_path;
	(void)output_readelf_path;
	(void)output_objdump_path;
#ifdef OUTPUT_DLL_INFO
	output_dll_info(output_dll_path, output_xxd_path, output_readelf_path, output_objdump_path);
#endif

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
	char *nasm_path,
	char *expected_dll_path,
	char *nasm_o_path,
	char *expected_xxd_path,
	char *expected_readelf_path,
	char *expected_objdump_path
) {
#ifdef __x86_64__
	run((char *[]){"nasm", nasm_path, "-felf64", "-O0", "-o", nasm_o_path, NULL});
	// run((char *[]){"nasm", nasm_path, "-felf64", "-g", "-O0", "-o", nasm_o_path, NULL});
	run((char *[]){"ld", nasm_o_path, "-o", expected_dll_path, "-x", "-shared", "--hash-style=sysv", NULL});
#elif __aarch64__
	run((char *[]){"nasm", nasm_path, "-fmacho64", "-O0", "-o", nasm_o_path, NULL});
	run((char *[]){"ld", nasm_o_path, "-o", expected_dll_path, "-x", "-dylib", NULL});
#else
#error Unsupported or unrecognized architecture
#endif

	(void)expected_xxd_path;
	(void)expected_readelf_path;
	(void)expected_objdump_path;
#ifdef OUTPUT_DLL_INFO
	output_dll_info(expected_dll_path, expected_xxd_path, expected_readelf_path, expected_objdump_path);
#endif
}

static struct test_data get_expected_test_data(
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

	regenerate_expected_dll(nasm_path, expected_dll_path, nasm_o_path, expected_xxd_path, expected_readelf_path, expected_objdump_path);

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
	init_globals(g, 42);
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

	return get_expected_test_data(nasm_path, results_path, expected_dll_path, nasm_o_path, expected_xxd_path, expected_readelf_path, expected_objdump_path, failed_file_path, expected_define_type, expected_globals_size);
}

static bool had_runtime_error = false;
static enum grug_runtime_error_type runtime_error_type = -1;
static char *runtime_error_on_fn_name = NULL;
static char *runtime_error_on_fn_path = NULL;
static void runtime_error_handler(char *reason, enum grug_runtime_error_type type, char *on_fn_name, char *on_fn_path) {
	had_runtime_error = true;

	runtime_error_reason = reason;
	runtime_error_type = type;
	runtime_error_on_fn_name = on_fn_name;
	runtime_error_on_fn_path = on_fn_path;
}

char *grug_get_runtime_error_reason(enum grug_runtime_error_type type);

static void runtime_error_time_limit_exceeded(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(streq(runtime_error_reason, grug_get_runtime_error_reason(GRUG_ON_FN_TIME_LIMIT_EXCEEDED)));

	assert(runtime_error_type == GRUG_ON_FN_TIME_LIMIT_EXCEEDED);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/time_limit_exceeded/input.grug"));

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/err_runtime/time_limit_exceeded/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_all(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(streq(runtime_error_reason, grug_get_runtime_error_reason(GRUG_ON_FN_DIVISION_BY_ZERO)));

	assert(runtime_error_type == GRUG_ON_FN_DIVISION_BY_ZERO);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/all/input.grug"));

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/err_runtime/all/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_division_by_0(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(streq(runtime_error_reason, grug_get_runtime_error_reason(GRUG_ON_FN_DIVISION_BY_ZERO)));

	assert(runtime_error_type == GRUG_ON_FN_DIVISION_BY_ZERO);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/division_by_0/input.grug"));

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/err_runtime/division_by_0/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_remainder_by_0(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(streq(runtime_error_reason, grug_get_runtime_error_reason(GRUG_ON_FN_DIVISION_BY_ZERO)));

	assert(runtime_error_type == GRUG_ON_FN_DIVISION_BY_ZERO);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/remainder_by_0/input.grug"));

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/err_runtime/remainder_by_0/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_stack_overflow(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(streq(runtime_error_reason, grug_get_runtime_error_reason(GRUG_ON_FN_STACK_OVERFLOW)));

	assert(runtime_error_type == GRUG_ON_FN_STACK_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/stack_overflow/input.grug"));

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/err_runtime/stack_overflow/input.grug"));

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

	return get_expected_test_data(nasm_path, results_path, expected_dll_path, nasm_o_path, expected_xxd_path, expected_readelf_path, expected_objdump_path, failed_file_path, expected_define_type, expected_globals_size);
}

static void ok_addition_as_argument(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 3);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/addition_as_argument/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/addition_as_two_arguments/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/addition_i32_wraparound/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/addition_with_multiplication/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/addition_with_multiplication_2/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/and_false_1/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/and_false_2/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/and_false_3/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/and_short_circuit/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/and_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/blocked_alrm/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/bool_logical_not_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/bool_logical_not_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/bool_returned/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/break/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/calls_100/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/calls_1000/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/comment_above_block/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/comment_above_block_twice/input.grug"));

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

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	assert(((int32_t*)globals)[0] == 420);
	assert(((int32_t*)globals)[1] == 1337);
	assert(((int32_t*)globals)[2] == 100);

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/comment_above_helper_fn/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_above_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/comment_above_on_fn/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_between_globals(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	assert(((int32_t*)globals)[0] == 420);
	assert(((int32_t*)globals)[1] == 1337);

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/comment_between_statements/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/comment_lone_block/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_lone_block_at_end(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/comment_lone_block_at_end/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_lone_global(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/comment_lone_global/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/continue/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/division_negative_result/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/division_positive_result/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/else_after_else_if_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/else_after_else_if_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/else_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/else_if_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/else_if_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/else_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/empty_line/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/entity_and_on_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/entity_and_resource_as_subexpression/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/eq_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/eq_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_addition/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_argument/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_division/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_eq_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_eq_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_ge_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_ge_true_1/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_ge_true_2/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_global_variable/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_gt_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_gt_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_le_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_le_true_1/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_le_true_2/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_local_variable/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_lt_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_lt_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_multiplication/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_ne_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_negated/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_ne_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_passed_to_helper_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_passed_to_on_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_passing_sin_to_cos/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/f32_subtraction/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/fibonacci/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/ge_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/ge_true_1/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/ge_true_2/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_global_containing_addition(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	assert(((int32_t*)globals)[0] == 5);

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

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	assert(((int32_t*)globals)[0] == 420);
	assert(((int32_t*)globals)[1] == 1337);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_globals_1000(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	char *globals = g;
	assert(*((uint64_t *)globals) == 42);
	globals += sizeof(uint64_t);

	for (int32_t i = 0; i < 1000; i++) {
		assert(((int32_t *)globals)[i] == i + 1);
	}

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_globals_1000_string(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	char *globals = g;
	assert(*((uint64_t *)globals) == 42);
	globals += sizeof(uint64_t);

	static char expected[sizeof("global0001")];
	memcpy(expected, "global", 6);

	for (int32_t i = 0; i < 1000; i++) {
		snprintf(expected + sizeof("global") - 1, sizeof("0001"), "%.4d", i + 1);
		assert(streq(((char **)globals)[i], expected));
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

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	for (int32_t i = 0; i < 32; i++) {
		assert(((int32_t*)globals)[i] == i + 1);
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

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	for (int32_t i = 0; i < 64; i++) {
		assert(((int32_t*)globals)[i] == i + 1);
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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/gt_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/gt_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/helper_fn/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_helper_fn_called_in_if(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/helper_fn_called_in_if/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_helper_fn_called_indirectly(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/helper_fn_called_indirectly/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/helper_fn_overwriting_param/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/helper_fn_returning_void_has_no_return/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/helper_fn_returning_void_returns_void/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/i32_max/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/i32_min/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/i32_negated/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_id_local_variable_get_and_set(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_get_opponent_call_count == 0);
	assert(game_fn_set_target_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_get_opponent_call_count == 1);
	assert(game_fn_set_target_call_count == 1);

	free(g);

	assert(game_fn_set_target_target == 42);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/id_local_variable_get_and_set/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/if_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/if_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/le_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/le_true_1/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/le_true_2/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/lt_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/lt_true/input.grug"));

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
	assert(game_fn_mega_id == 42);
	assert(streq(game_fn_mega_str, "foo"));

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/max_args/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_me(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_target_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_target_call_count == 1);

	free(g);

	assert(game_fn_set_target_target == 42);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/me/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_me_assigned_to_local_variable(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_target_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_target_call_count == 1);

	free(g);

	assert(game_fn_set_target_target == 42);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/me_assigned_to_local_variable/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_me_passed_to_helper_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_target_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_target_call_count == 1);

	free(g);

	assert(game_fn_set_target_target == 42);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/me_passed_to_helper_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/multiplication_as_two_arguments/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/ne_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/negate_parenthesized_expr/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/negative_literal/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/nested_break/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/nested_continue/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/ne_true/input.grug"));

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

static void ok_null_id_initializing_global(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_target_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_target_call_count == 1);

	free(g);

	assert(game_fn_set_target_target == UINT64_MAX);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/null_id_initializing_global/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_null_id_initializing_local(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_target_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_target_call_count == 1);

	free(g);

	assert(game_fn_set_target_target == UINT64_MAX);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/null_id_initializing_local/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_no_empty_line_between_globals(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	assert(((int32_t*)globals)[0] == 420);
	assert(((int32_t*)globals)[1] == 1337);

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/no_empty_line_between_statements/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_calling_game_fn_nothing/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_calling_game_fn_nothing_twice/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_calling_game_fn_plt_order/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_calling_helper_fns/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_overwriting_param/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_passing_argument_to_helper_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_passing_magic_to_initialize/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_three(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);
	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_three/input.grug"));

	((struct j_on_fns *)on_fns)->b(g);
	assert(streq(grug_on_fn_name, "on_b"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_three/input.grug"));

	((struct j_on_fns *)on_fns)->c(g);
	assert(streq(grug_on_fn_name, "on_c"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_three/input.grug"));

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
	assert(streq(grug_on_fn_name, "on_b"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_three_unused_first/input.grug"));

	((struct j_on_fns *)on_fns)->c(g);
	assert(streq(grug_on_fn_name, "on_c"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_three_unused_first/input.grug"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_three_unused_second(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);
	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_three_unused_second/input.grug"));

	assert(((struct j_on_fns *)on_fns)->b == NULL);

	((struct j_on_fns *)on_fns)->c(g);
	assert(streq(grug_on_fn_name, "on_c"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_three_unused_second/input.grug"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_three_unused_third(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);
	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_three_unused_third/input.grug"));

	((struct j_on_fns *)on_fns)->b(g);
	assert(streq(grug_on_fn_name, "on_b"));
	assert(streq(grug_on_fn_path, "tests/ok/on_fn_three_unused_third/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/or_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/or_short_circuit/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/or_true_1/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/or_true_2/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/or_true_3/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/pass_string_argument_to_game_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/pass_string_argument_to_helper_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/remainder_negative_result/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/remainder_positive_result/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/resource_and_entity_and_on_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/resource_and_on_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/resource_in_define_and_entity_in_game_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/return/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/return_from_on_fn/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_return_from_on_fn_minimal(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/return_from_on_fn_minimal/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/return_with_no_value/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_same_variable_name_in_different_functions(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 2);

	free(g);

	assert(game_fn_initialize_x == 69);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/same_variable_name_in_different_functions/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/stack_16_byte_alignment/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/stack_16_byte_alignment_midway/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_spill_args_to_define_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_define_c2_i1 == 1);
	assert(game_fn_define_c2_i2 == 2);
	assert(game_fn_define_c2_i3 == 3);
	assert(game_fn_define_c2_i4 == 4);
	assert(game_fn_define_c2_i5 == 5);
	assert(game_fn_define_c2_i6 == 6);
	assert(game_fn_define_c2_i7 == 7);
	assert(game_fn_define_c2_f1 == 1.0f);
	assert(game_fn_define_c2_f2 == 2.0f);
	assert(game_fn_define_c2_f3 == 3.0f);
	assert(game_fn_define_c2_f4 == 4.0f);
	assert(game_fn_define_c2_f5 == 5.0f);
	assert(game_fn_define_c2_f6 == 6.0f);
	assert(game_fn_define_c2_f7 == 7.0f);
	assert(game_fn_define_c2_f8 == 8.0f);
	assert(game_fn_define_c2_f9 == 9.0f);
	assert(game_fn_define_c2_i8 == 8);
	assert(game_fn_define_c2_f10 == 10.0f);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_spill_args_to_define_fn_subless(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_define_d2_i1 == 1);
	assert(game_fn_define_d2_i2 == 2);
	assert(game_fn_define_d2_i3 == 3);
	assert(game_fn_define_d2_i4 == 4);
	assert(game_fn_define_d2_i5 == 5);
	assert(game_fn_define_d2_i6 == 6);
	assert(game_fn_define_d2_i7 == 7);
	assert(game_fn_define_d2_f1 == 1.0f);
	assert(game_fn_define_d2_f2 == 2.0f);
	assert(game_fn_define_d2_f3 == 3.0f);
	assert(game_fn_define_d2_f4 == 4.0f);
	assert(game_fn_define_d2_f5 == 5.0f);
	assert(game_fn_define_d2_f6 == 6.0f);
	assert(game_fn_define_d2_f7 == 7.0f);
	assert(game_fn_define_d2_f8 == 8.0f);
	assert(game_fn_define_d2_i8 == 8);
	assert(game_fn_define_d2_f9 == 9.0f);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_spill_args_to_game_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_motherload_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_motherload_call_count == 1);

	free(g);

	assert(game_fn_motherload_i1 == 1);
	assert(game_fn_motherload_i2 == 2);
	assert(game_fn_motherload_i3 == 3);
	assert(game_fn_motherload_i4 == 4);
	assert(game_fn_motherload_i5 == 5);
	assert(game_fn_motherload_i6 == 6);
	assert(game_fn_motherload_i7 == 7);
	assert(game_fn_motherload_f1 == 1.0f);
	assert(game_fn_motherload_f2 == 2.0f);
	assert(game_fn_motherload_f3 == 3.0f);
	assert(game_fn_motherload_f4 == 4.0f);
	assert(game_fn_motherload_f5 == 5.0f);
	assert(game_fn_motherload_f6 == 6.0f);
	assert(game_fn_motherload_f7 == 7.0f);
	assert(game_fn_motherload_f8 == 8.0f);
	assert(game_fn_motherload_id == 42);
	assert(game_fn_motherload_f9 == 9.0f);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/spill_args_to_game_fn/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_spill_args_to_game_fn_subless(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_motherload_subless_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_motherload_subless_call_count == 1);

	free(g);

	assert(game_fn_motherload_subless_i1 == 1);
	assert(game_fn_motherload_subless_i2 == 2);
	assert(game_fn_motherload_subless_i3 == 3);
	assert(game_fn_motherload_subless_i4 == 4);
	assert(game_fn_motherload_subless_i5 == 5);
	assert(game_fn_motherload_subless_i6 == 6);
	assert(game_fn_motherload_subless_i7 == 7);
	assert(game_fn_motherload_subless_f1 == 1.0f);
	assert(game_fn_motherload_subless_f2 == 2.0f);
	assert(game_fn_motherload_subless_f3 == 3.0f);
	assert(game_fn_motherload_subless_f4 == 4.0f);
	assert(game_fn_motherload_subless_f5 == 5.0f);
	assert(game_fn_motherload_subless_f6 == 6.0f);
	assert(game_fn_motherload_subless_f7 == 7.0f);
	assert(game_fn_motherload_subless_f8 == 8.0f);
	assert(game_fn_motherload_subless_f9 == 9.0f);
	assert(game_fn_motherload_subless_id == 42);
	assert(game_fn_motherload_subless_f10 == 10.0f);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/spill_args_to_game_fn_subless/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_spill_args_to_helper_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_motherload_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_motherload_call_count == 1);

	free(g);

	assert(game_fn_motherload_i1 == 1);
	assert(game_fn_motherload_i2 == 2);
	assert(game_fn_motherload_i3 == 3);
	assert(game_fn_motherload_i4 == 4);
	assert(game_fn_motherload_i5 == 5);
	assert(game_fn_motherload_i6 == 6);
	assert(game_fn_motherload_i7 == 7);
	assert(game_fn_motherload_f1 == 1.0f);
	assert(game_fn_motherload_f2 == 2.0f);
	assert(game_fn_motherload_f3 == 3.0f);
	assert(game_fn_motherload_f4 == 4.0f);
	assert(game_fn_motherload_f5 == 5.0f);
	assert(game_fn_motherload_f6 == 6.0f);
	assert(game_fn_motherload_f7 == 7.0f);
	assert(game_fn_motherload_f8 == 8.0f);
	assert(game_fn_motherload_id == 42);
	assert(game_fn_motherload_f9 == 9.0f);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/spill_args_to_helper_fn/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_spill_args_to_helper_fn_32_bit_f32(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_offset_32_bit_f32_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_offset_32_bit_f32_call_count == 1);

	free(g);

	assert(streq(game_fn_offset_32_bit_f32_s1, "1"));
	assert(streq(game_fn_offset_32_bit_f32_s2, "2"));
	assert(streq(game_fn_offset_32_bit_f32_s3, "3"));
	assert(streq(game_fn_offset_32_bit_f32_s4, "4"));
	assert(streq(game_fn_offset_32_bit_f32_s5, "5"));
	assert(streq(game_fn_offset_32_bit_f32_s6, "6"));
	assert(streq(game_fn_offset_32_bit_f32_s7, "7"));
	assert(streq(game_fn_offset_32_bit_f32_s8, "8"));
	assert(streq(game_fn_offset_32_bit_f32_s9, "9"));
	assert(streq(game_fn_offset_32_bit_f32_s10, "10"));
	assert(streq(game_fn_offset_32_bit_f32_s11, "11"));
	assert(streq(game_fn_offset_32_bit_f32_s12, "12"));
	assert(streq(game_fn_offset_32_bit_f32_s13, "13"));
	assert(streq(game_fn_offset_32_bit_f32_s14, "14"));
	assert(streq(game_fn_offset_32_bit_f32_s15, "15"));
	assert(game_fn_offset_32_bit_f32_f1 == 1.0f);
	assert(game_fn_offset_32_bit_f32_f2 == 2.0f);
	assert(game_fn_offset_32_bit_f32_f3 == 3.0f);
	assert(game_fn_offset_32_bit_f32_f4 == 4.0f);
	assert(game_fn_offset_32_bit_f32_f5 == 5.0f);
	assert(game_fn_offset_32_bit_f32_f6 == 6.0f);
	assert(game_fn_offset_32_bit_f32_f7 == 7.0f);
	assert(game_fn_offset_32_bit_f32_f8 == 8.0f);
	assert(game_fn_offset_32_bit_f32_g == 1);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/spill_args_to_helper_fn_32_bit_f32/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_spill_args_to_helper_fn_32_bit_i32(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_offset_32_bit_i32_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_offset_32_bit_i32_call_count == 1);

	free(g);

	assert(game_fn_offset_32_bit_i32_f1 == 1.0f);
	assert(game_fn_offset_32_bit_i32_f2 == 2.0f);
	assert(game_fn_offset_32_bit_i32_f3 == 3.0f);
	assert(game_fn_offset_32_bit_i32_f4 == 4.0f);
	assert(game_fn_offset_32_bit_i32_f5 == 5.0f);
	assert(game_fn_offset_32_bit_i32_f6 == 6.0f);
	assert(game_fn_offset_32_bit_i32_f7 == 7.0f);
	assert(game_fn_offset_32_bit_i32_f8 == 8.0f);
	assert(game_fn_offset_32_bit_i32_f9 == 9.0f);
	assert(game_fn_offset_32_bit_i32_f10 == 10.0f);
	assert(game_fn_offset_32_bit_i32_f11 == 11.0f);
	assert(game_fn_offset_32_bit_i32_f12 == 12.0f);
	assert(game_fn_offset_32_bit_i32_f13 == 13.0f);
	assert(game_fn_offset_32_bit_i32_f14 == 14.0f);
	assert(game_fn_offset_32_bit_i32_f15 == 15.0f);
	assert(game_fn_offset_32_bit_i32_f16 == 16.0f);
	assert(game_fn_offset_32_bit_i32_f17 == 17.0f);
	assert(game_fn_offset_32_bit_i32_f18 == 18.0f);
	assert(game_fn_offset_32_bit_i32_f19 == 19.0f);
	assert(game_fn_offset_32_bit_i32_f20 == 20.0f);
	assert(game_fn_offset_32_bit_i32_f21 == 21.0f);
	assert(game_fn_offset_32_bit_i32_f22 == 22.0f);
	assert(game_fn_offset_32_bit_i32_f23 == 23.0f);
	assert(game_fn_offset_32_bit_i32_f24 == 24.0f);
	assert(game_fn_offset_32_bit_i32_f25 == 25.0f);
	assert(game_fn_offset_32_bit_i32_f26 == 26.0f);
	assert(game_fn_offset_32_bit_i32_f27 == 27.0f);
	assert(game_fn_offset_32_bit_i32_f28 == 28.0f);
	assert(game_fn_offset_32_bit_i32_f29 == 29.0f);
	assert(game_fn_offset_32_bit_i32_f30 == 30.0f);
	assert(game_fn_offset_32_bit_i32_i1 == 1);
	assert(game_fn_offset_32_bit_i32_i2 == 2);
	assert(game_fn_offset_32_bit_i32_i3 == 3);
	assert(game_fn_offset_32_bit_i32_i4 == 4);
	assert(game_fn_offset_32_bit_i32_i5 == 5);
	assert(game_fn_offset_32_bit_i32_g == 6);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/spill_args_to_helper_fn_32_bit_i32/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_spill_args_to_helper_fn_32_bit_string(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_offset_32_bit_string_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_offset_32_bit_string_call_count == 1);

	free(g);

	assert(game_fn_offset_32_bit_string_f1 == 1.0f);
	assert(game_fn_offset_32_bit_string_f2 == 2.0f);
	assert(game_fn_offset_32_bit_string_f3 == 3.0f);
	assert(game_fn_offset_32_bit_string_f4 == 4.0f);
	assert(game_fn_offset_32_bit_string_f5 == 5.0f);
	assert(game_fn_offset_32_bit_string_f6 == 6.0f);
	assert(game_fn_offset_32_bit_string_f7 == 7.0f);
	assert(game_fn_offset_32_bit_string_f8 == 8.0f);
	assert(game_fn_offset_32_bit_string_f9 == 9.0f);
	assert(game_fn_offset_32_bit_string_f10 == 10.0f);
	assert(game_fn_offset_32_bit_string_f11 == 11.0f);
	assert(game_fn_offset_32_bit_string_f12 == 12.0f);
	assert(game_fn_offset_32_bit_string_f13 == 13.0f);
	assert(game_fn_offset_32_bit_string_f14 == 14.0f);
	assert(game_fn_offset_32_bit_string_f15 == 15.0f);
	assert(game_fn_offset_32_bit_string_f16 == 16.0f);
	assert(game_fn_offset_32_bit_string_f17 == 17.0f);
	assert(game_fn_offset_32_bit_string_f18 == 18.0f);
	assert(game_fn_offset_32_bit_string_f19 == 19.0f);
	assert(game_fn_offset_32_bit_string_f20 == 20.0f);
	assert(game_fn_offset_32_bit_string_f21 == 21.0f);
	assert(game_fn_offset_32_bit_string_f22 == 22.0f);
	assert(game_fn_offset_32_bit_string_f23 == 23.0f);
	assert(game_fn_offset_32_bit_string_f24 == 24.0f);
	assert(game_fn_offset_32_bit_string_f25 == 25.0f);
	assert(game_fn_offset_32_bit_string_f26 == 26.0f);
	assert(game_fn_offset_32_bit_string_f27 == 27.0f);
	assert(game_fn_offset_32_bit_string_f28 == 28.0f);
	assert(game_fn_offset_32_bit_string_f29 == 29.0f);
	assert(game_fn_offset_32_bit_string_f30 == 30.0f);
	assert(streq(game_fn_offset_32_bit_string_s1, "1"));
	assert(streq(game_fn_offset_32_bit_string_s2, "2"));
	assert(streq(game_fn_offset_32_bit_string_s3, "3"));
	assert(streq(game_fn_offset_32_bit_string_s4, "4"));
	assert(streq(game_fn_offset_32_bit_string_s5, "5"));
	assert(game_fn_offset_32_bit_string_g == 1);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/spill_args_to_helper_fn_32_bit_string/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_spill_args_to_helper_fn_subless(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_motherload_subless_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_motherload_subless_call_count == 1);

	free(g);

	assert(game_fn_motherload_subless_i1 == 1);
	assert(game_fn_motherload_subless_i2 == 2);
	assert(game_fn_motherload_subless_i3 == 3);
	assert(game_fn_motherload_subless_i4 == 4);
	assert(game_fn_motherload_subless_i5 == 5);
	assert(game_fn_motherload_subless_i6 == 6);
	assert(game_fn_motherload_subless_i7 == 7);
	assert(game_fn_motherload_subless_f1 == 1.0f);
	assert(game_fn_motherload_subless_f2 == 2.0f);
	assert(game_fn_motherload_subless_f3 == 3.0f);
	assert(game_fn_motherload_subless_f4 == 4.0f);
	assert(game_fn_motherload_subless_f5 == 5.0f);
	assert(game_fn_motherload_subless_f6 == 6.0f);
	assert(game_fn_motherload_subless_f7 == 7.0f);
	assert(game_fn_motherload_subless_f8 == 8.0f);
	assert(game_fn_motherload_subless_f9 == 9.0f);
	assert(game_fn_motherload_subless_id == 42);
	assert(game_fn_motherload_subless_f10 == 10.0f);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/spill_args_to_helper_fn_subless/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/string_and_on_fn/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_string_can_be_passed_to_helper_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_say_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_say_call_count == 1);

	free(g);

	assert(streq(game_fn_say_message, "foo"));

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/string_can_be_passed_to_helper_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/string_eq_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/string_eq_true/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/string_eq_true_empty/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/string_ne_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/string_ne_false_empty/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/string_ne_true/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_sub_rsp_32_bits_global_variables_i32(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 65);

	free(g);

	assert(game_fn_initialize_x == 65);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/sub_rsp_32_bits_global_variables_i32/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_sub_rsp_32_bits_global_variables_id(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_target_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_target_call_count == 65);

	free(g);

	assert(game_fn_set_target_target == UINT64_MAX);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/sub_rsp_32_bits_global_variables_id/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_sub_rsp_32_bits_local_variables_i32(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 65);

	free(g);

	assert(game_fn_initialize_x == 65);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/sub_rsp_32_bits_local_variables_i32/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_sub_rsp_32_bits_local_variables_id(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_target_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_target_call_count == 65);

	free(g);

	assert(game_fn_set_target_target == UINT64_MAX);

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/sub_rsp_32_bits_local_variables_id/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/subtraction_negative_result/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/subtraction_positive_result/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/variable/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/variable_does_not_shadow_define_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/variable_reassignment/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/variable_shadows_define_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/variable_shadows_game_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/variable_shadows_helper_fn/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/variable_shadows_on_fn_1/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/variable_shadows_on_fn_2/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/variable_string_global/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/variable_string_local/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/void_function_early_return/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/while_false/input.grug"));

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

	assert(streq(grug_on_fn_name, "on_a"));
	assert(streq(grug_on_fn_path, "tests/ok/write_to_global_variable/input.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void add_error_tests(void) {
	ADD_TEST_ERROR(assign_to_unknown_variable);
	ADD_TEST_ERROR(assignment_isnt_expression);
	ADD_TEST_ERROR(bool_cant_be_initialized_with_1);
	ADD_TEST_ERROR(bool_unary_minus);
	ADD_TEST_ERROR(cant_add_strings);
	ADD_TEST_ERROR(cant_break_outside_of_loop);
	ADD_TEST_ERROR(cant_call_define_fn_1);
	ADD_TEST_ERROR(cant_call_define_fn_2);
	ADD_TEST_ERROR(cant_continue_outside_of_loop);
	ADD_TEST_ERROR(cant_declare_me_globally);
	ADD_TEST_ERROR(cant_declare_me_locally);
	ADD_TEST_ERROR(cant_declare_variable_in_fn_call);
	ADD_TEST_ERROR(cant_redefine_global);
	ADD_TEST_ERROR(comment_at_the_end_of_another_statement);
	ADD_TEST_ERROR(comment_at_the_end_of_define);
	ADD_TEST_ERROR(comment_lone_global_at_end);
	ADD_TEST_ERROR(define_fn_calls_fn);
	ADD_TEST_ERROR(define_fn_different_name);
	ADD_TEST_ERROR(define_fn_different_type);
	ADD_TEST_ERROR(define_fn_not_enough_arguments);
	ADD_TEST_ERROR(define_fn_only_one_max);
	ADD_TEST_ERROR(define_fn_uses_global_variable);
	ADD_TEST_ERROR(define_fn_was_not_declared);
	ADD_TEST_ERROR(empty_line_after_group);
	ADD_TEST_ERROR(empty_line_at_start_of_file);
	ADD_TEST_ERROR(empty_line_before_group);
	ADD_TEST_ERROR(empty_line_fn_group);
	ADD_TEST_ERROR(empty_line_twice_at_end_of_file);
	ADD_TEST_ERROR(empty_line_twice_between_local_statements);
	ADD_TEST_ERROR(empty_line_while_group);
	ADD_TEST_ERROR(entity_cant_be_empty_string);
	ADD_TEST_ERROR(entity_cant_be_passed_to_helper_fn);
	ADD_TEST_ERROR(entity_has_invalid_entity_name_colon);
	ADD_TEST_ERROR(entity_has_invalid_entity_name_uppercase);
	ADD_TEST_ERROR(entity_has_invalid_mod_name_uppercase);
	ADD_TEST_ERROR(entity_mod_name_and_entity_name_is_missing);
	ADD_TEST_ERROR(entity_mod_name_cant_be_current_mod);
	ADD_TEST_ERROR(entity_mod_name_is_missing);
	ADD_TEST_ERROR(entity_name_is_missing);
	ADD_TEST_ERROR(f32_missing_digit_after_decimal_point);
	ADD_TEST_ERROR(game_fn_does_not_exist);
	ADD_TEST_ERROR(game_function_call_gets_wrong_arg_type);
	ADD_TEST_ERROR(game_function_call_less_args_expected);
	ADD_TEST_ERROR(game_function_call_more_args_expected);
	ADD_TEST_ERROR(game_function_call_no_args_expected);
	ADD_TEST_ERROR(global_variable_already_uses_local_variable_name);
	ADD_TEST_ERROR(global_variable_before_define);
	ADD_TEST_ERROR(global_variable_calls_fn);
	ADD_TEST_ERROR(global_variable_definition_cant_use_itself);
	ADD_TEST_ERROR(global_variable_definition_missing_type);
	ADD_TEST_ERROR(global_variable_definition_requires_value_i32);
	ADD_TEST_ERROR(global_variable_definition_requires_value_string);
	ADD_TEST_ERROR(global_variable_uses_global_variable);
	ADD_TEST_ERROR(helper_fn_defined_before_first_helper_fn_usage);
	ADD_TEST_ERROR(helper_fn_defined_between_on_fns);
	ADD_TEST_ERROR(helper_fn_does_not_exist);
	ADD_TEST_ERROR(helper_fn_is_not_called_1);
	ADD_TEST_ERROR(helper_fn_is_not_called_2);
	ADD_TEST_ERROR(helper_fn_is_not_called_3);
	ADD_TEST_ERROR(helper_fn_is_not_called_4);
	ADD_TEST_ERROR(helper_fn_is_not_called_5);
	ADD_TEST_ERROR(helper_function_call_gets_wrong_arg_type);
	ADD_TEST_ERROR(helper_function_call_less_args_expected);
	ADD_TEST_ERROR(helper_function_call_more_args_expected);
	ADD_TEST_ERROR(helper_function_call_no_args_expected);
	ADD_TEST_ERROR(helper_function_different_return_value_expected);
	ADD_TEST_ERROR(helper_function_missing_return_statement);
	ADD_TEST_ERROR(helper_function_no_return_value_expected);
	ADD_TEST_ERROR(i32_logical_not);
	ADD_TEST_ERROR(i32_too_big);
	ADD_TEST_ERROR(i32_too_small);
	ADD_TEST_ERROR(indented_call_argument);
	ADD_TEST_ERROR(indented_call_arguments);
	ADD_TEST_ERROR(indented_helper_fn_parameter);
	ADD_TEST_ERROR(indented_helper_fn_parameters);
	ADD_TEST_ERROR(indented_on_fn_parameter);
	ADD_TEST_ERROR(indented_on_fn_parameters);
	ADD_TEST_ERROR(indentation_going_down_by_2);
	ADD_TEST_ERROR(local_variable_already_exists);
	ADD_TEST_ERROR(local_variable_definition_cant_use_itself);
	ADD_TEST_ERROR(local_variable_definition_missing_type);
	ADD_TEST_ERROR(me_cant_be_assigned_to_global);
	ADD_TEST_ERROR(me_cant_be_written_to);
	ADD_TEST_ERROR(me_plus_1);
	ADD_TEST_ERROR(me_plus_me);
	ADD_TEST_ERROR(missing_define_fn);
	ADD_TEST_ERROR(missing_empty_line_between_define_fn_and_global);
	ADD_TEST_ERROR(missing_empty_line_between_global_and_on_fn);
	ADD_TEST_ERROR(missing_empty_line_between_on_fn_and_helper_fn);
	ADD_TEST_ERROR(no_space_between_comment_character_and_comment);
	ADD_TEST_ERROR(null_id_plus_1);
	ADD_TEST_ERROR(null_id_plus_null_id);
	ADD_TEST_ERROR(on_fn_before_define);
	ADD_TEST_ERROR(on_fn_cant_be_called_by_helper_fn);
	ADD_TEST_ERROR(on_fn_cant_be_called_by_on_fn);
	ADD_TEST_ERROR(on_fn_defined_after_helper_fn);
	ADD_TEST_ERROR(on_fn_duplicate);
	ADD_TEST_ERROR(on_fn_was_not_declared_in_entity);
	ADD_TEST_ERROR(on_fn_wrong_order);
	ADD_TEST_ERROR(on_function_gets_wrong_arg_type);
	ADD_TEST_ERROR(on_function_less_args_expected);
	ADD_TEST_ERROR(on_function_more_args_expected);
	ADD_TEST_ERROR(on_function_no_args_expected);
	ADD_TEST_ERROR(on_function_no_return_value_expected);
	ADD_TEST_ERROR(pass_bool_to_i32_game_param);
	ADD_TEST_ERROR(pass_bool_to_i32_helper_param);
	ADD_TEST_ERROR(remainder_by_float);
	ADD_TEST_ERROR(resource_cant_be_empty_string);
	ADD_TEST_ERROR(resource_cant_be_passed_to_helper_fn);
	ADD_TEST_ERROR(resource_cant_contain_backslash);
	ADD_TEST_ERROR(resource_cant_contain_double_slash);
	ADD_TEST_ERROR(resource_cant_go_up_to_parent_directory_1);
	ADD_TEST_ERROR(resource_cant_go_up_to_parent_directory_2);
	ADD_TEST_ERROR(resource_cant_go_up_to_parent_directory_3);
	ADD_TEST_ERROR(resource_cant_go_up_to_parent_directory_4);
	ADD_TEST_ERROR(resource_cant_have_leading_slash);
	ADD_TEST_ERROR(resource_cant_have_trailing_slash);
	ADD_TEST_ERROR(resource_cant_refer_to_current_directory_1);
	ADD_TEST_ERROR(resource_cant_refer_to_current_directory_2);
	ADD_TEST_ERROR(resource_cant_refer_to_current_directory_3);
	ADD_TEST_ERROR(resource_cant_refer_to_current_directory_4);
	ADD_TEST_ERROR(resource_type_for_global);
	ADD_TEST_ERROR(resource_type_for_helper_fn_argument);
	ADD_TEST_ERROR(resource_type_for_helper_fn_return_type);
	ADD_TEST_ERROR(resource_type_for_local);
	ADD_TEST_ERROR(resource_type_for_on_fn_argument);
	ADD_TEST_ERROR(string_pointer_arithmetic);
	ADD_TEST_ERROR(trailing_space_in_comment);
	ADD_TEST_ERROR(unclosed_double_quote);
	ADD_TEST_ERROR(unknown_variable);
	ADD_TEST_ERROR(unused_expr_result);
	ADD_TEST_ERROR(variable_assignment_before_definition);
	ADD_TEST_ERROR(variable_definition_requires_value_i32);
	ADD_TEST_ERROR(variable_definition_requires_value_string);
	ADD_TEST_ERROR(variable_statement_missing_assignment);
	ADD_TEST_ERROR(variable_used_before_definition);
	ADD_TEST_ERROR(wrong_type_global_assignment);
	ADD_TEST_ERROR(wrong_type_global_reassignment);
	ADD_TEST_ERROR(wrong_type_local_assignment);
	ADD_TEST_ERROR(wrong_type_local_reassignment);
}

static void add_runtime_error_tests(void) {
	ADD_TEST_RUNTIME_ERROR(all, "d", 8);
	ADD_TEST_RUNTIME_ERROR(division_by_0, "d", 8);
	ADD_TEST_RUNTIME_ERROR(remainder_by_0, "d", 8);
	ADD_TEST_RUNTIME_ERROR(stack_overflow, "d", 8);
	ADD_TEST_RUNTIME_ERROR(time_limit_exceeded, "d", 8);
}

static void add_ok_tests(void) {
	ADD_TEST_OK(addition_as_argument, "d", 8);
	ADD_TEST_OK(addition_as_two_arguments, "d", 8);
	ADD_TEST_OK(addition_i32_wraparound, "d", 8);
	ADD_TEST_OK(addition_with_multiplication, "d", 8);
	ADD_TEST_OK(addition_with_multiplication_2, "d", 8);
	ADD_TEST_OK(and_false_1, "d", 8);
	ADD_TEST_OK(and_false_2, "d", 8);
	ADD_TEST_OK(and_false_3, "d", 8);
	ADD_TEST_OK(and_short_circuit, "d", 8);
	ADD_TEST_OK(and_true, "d", 8);
	ADD_TEST_OK(blocked_alrm, "d", 8);
	ADD_TEST_OK(bool_logical_not_false, "d", 8);
	ADD_TEST_OK(bool_logical_not_true, "d", 8);
	ADD_TEST_OK(bool_returned, "d", 8);
	ADD_TEST_OK(break, "d", 8);
	ADD_TEST_OK(calls_100, "d", 8);
	ADD_TEST_OK(calls_1000, "d", 8);
	ADD_TEST_OK(comment_above_block, "d", 8);
	ADD_TEST_OK(comment_above_block_twice, "d", 8);
	ADD_TEST_OK(comment_above_define_fn, "d", 8);
	ADD_TEST_OK(comment_above_define_fn_twice, "d", 8);
	ADD_TEST_OK(comment_above_globals, "d", 20);
	ADD_TEST_OK(comment_above_helper_fn, "d", 8);
	ADD_TEST_OK(comment_above_on_fn, "d", 8);
	ADD_TEST_OK(comment_between_globals, "a", 16);
	ADD_TEST_OK(comment_between_statements, "d", 8);
	ADD_TEST_OK(comment_lone_block, "d", 8);
	ADD_TEST_OK(comment_lone_block_at_end, "d", 8);
	ADD_TEST_OK(comment_lone_global, "d", 8);
	ADD_TEST_OK(continue, "d", 8);
	ADD_TEST_OK(define, "h", 8);
	ADD_TEST_OK(define_containing_addition, "b", 8);
	ADD_TEST_OK(define_containing_string, "k", 8);
	ADD_TEST_OK(define_with_eight_f32_fields, "t", 8);
	ADD_TEST_OK(define_with_six_fields, "m", 8);
	ADD_TEST_OK(define_with_six_i32_fields, "n", 8);
	ADD_TEST_OK(define_with_six_string_fields, "o", 8);
	ADD_TEST_OK(division_negative_result, "d", 8);
	ADD_TEST_OK(division_positive_result, "d", 8);
	ADD_TEST_OK(else_after_else_if_false, "d", 8);
	ADD_TEST_OK(else_after_else_if_true, "d", 8);
	ADD_TEST_OK(else_false, "d", 8);
	ADD_TEST_OK(else_if_false, "d", 8);
	ADD_TEST_OK(else_if_true, "d", 8);
	ADD_TEST_OK(else_true, "d", 8);
	ADD_TEST_OK(empty_line, "d", 8);
	ADD_TEST_OK(entity_and_on_fn, "z", 8);
	ADD_TEST_OK(entity_and_resource_as_subexpression, "d", 8);
	ADD_TEST_OK(entity_duplicate, "y", 8);
	ADD_TEST_OK(entity_in_define, "x", 8);
	ADD_TEST_OK(entity_in_define_with_mod_specified, "x", 8);
	ADD_TEST_OK(entity_twice, "y", 8);
	ADD_TEST_OK(eq_false, "d", 8);
	ADD_TEST_OK(eq_true, "d", 8);
	ADD_TEST_OK(f32_addition, "d", 8);
	ADD_TEST_OK(f32_argument, "d", 8);
	ADD_TEST_OK(f32_division, "d", 8);
	ADD_TEST_OK(f32_eq_false, "d", 8);
	ADD_TEST_OK(f32_eq_true, "d", 8);
	ADD_TEST_OK(f32_ge_false, "d", 8);
	ADD_TEST_OK(f32_ge_true_1, "d", 8);
	ADD_TEST_OK(f32_ge_true_2, "d", 8);
	ADD_TEST_OK(f32_global_variable, "d", 12);
	ADD_TEST_OK(f32_gt_false, "d", 8);
	ADD_TEST_OK(f32_gt_true, "d", 8);
	ADD_TEST_OK(f32_le_false, "d", 8);
	ADD_TEST_OK(f32_le_true_1, "d", 8);
	ADD_TEST_OK(f32_le_true_2, "d", 8);
	ADD_TEST_OK(f32_local_variable, "d", 8);
	ADD_TEST_OK(f32_lt_false, "d", 8);
	ADD_TEST_OK(f32_lt_true, "d", 8);
	ADD_TEST_OK(f32_multiplication, "d", 8);
	ADD_TEST_OK(f32_ne_false, "d", 8);
	ADD_TEST_OK(f32_negated, "d", 8);
	ADD_TEST_OK(f32_ne_true, "d", 8);
	ADD_TEST_OK(f32_passed_to_helper_fn, "d", 8);
	ADD_TEST_OK(f32_passed_to_on_fn, "r", 8);
	ADD_TEST_OK(f32_passing_sin_to_cos, "d", 8);
	ADD_TEST_OK(f32_subtraction, "d", 8);
	ADD_TEST_OK(fibonacci, "d", 8);
	ADD_TEST_OK(ge_false, "d", 8);
	ADD_TEST_OK(ge_true_1, "d", 8);
	ADD_TEST_OK(ge_true_2, "d", 8);
	ADD_TEST_OK(global_containing_addition, "a", 12);
	ADD_TEST_OK(globals, "h", 16);
	ADD_TEST_OK(globals_1000, "a", 4008);
	ADD_TEST_OK(globals_1000_string, "a", 8008);
	ADD_TEST_OK(globals_32, "a", 136);
	ADD_TEST_OK(globals_64, "a", 264);
	ADD_TEST_OK(gt_false, "d", 8);
	ADD_TEST_OK(gt_true, "d", 8);
	ADD_TEST_OK(helper_fn, "d", 8);
	ADD_TEST_OK(helper_fn_called_in_if, "d", 8);
	ADD_TEST_OK(helper_fn_called_indirectly, "d", 8);
	ADD_TEST_OK(helper_fn_overwriting_param, "d", 8);
	ADD_TEST_OK(helper_fn_returning_void_has_no_return, "d", 8);
	ADD_TEST_OK(helper_fn_returning_void_returns_void, "d", 8);
	ADD_TEST_OK(i32_max, "d", 8);
	ADD_TEST_OK(i32_min, "d", 8);
	ADD_TEST_OK(i32_negated, "d", 8);
	ADD_TEST_OK(id_local_variable_get_and_set, "d", 8);
	ADD_TEST_OK(identical_strings_are_shared, "q", 8);
	ADD_TEST_OK(if_false, "d", 8);
	ADD_TEST_OK(if_true, "d", 8);
	ADD_TEST_OK(le_false, "d", 8);
	ADD_TEST_OK(le_true_1, "d", 8);
	ADD_TEST_OK(le_true_2, "d", 8);
	ADD_TEST_OK(lt_false, "d", 8);
	ADD_TEST_OK(lt_true, "d", 8);
	ADD_TEST_OK(max_args, "d", 8);
	ADD_TEST_OK(me, "d", 8);
	ADD_TEST_OK(me_assigned_to_local_variable, "d", 8);
	ADD_TEST_OK(me_passed_to_helper_fn, "d", 8);
	ADD_TEST_OK(minimal, "a", 8);
	ADD_TEST_OK(multiplication_as_two_arguments, "d", 8);
	ADD_TEST_OK(ne_false, "d", 8);
	ADD_TEST_OK(negate_parenthesized_expr, "d", 8);
	ADD_TEST_OK(negative_literal, "d", 8);
	ADD_TEST_OK(nested_break, "d", 8);
	ADD_TEST_OK(nested_continue, "d", 8);
	ADD_TEST_OK(ne_true, "d", 8);
	ADD_TEST_OK(no_define_fields, "d", 8);
	ADD_TEST_OK(no_on_fns, "a", 8);
	ADD_TEST_OK(null_id_initializing_global, "d", 16);
	ADD_TEST_OK(null_id_initializing_local, "d", 8);
	ADD_TEST_OK(no_empty_line_between_globals, "a", 16);
	ADD_TEST_OK(no_empty_line_between_statements, "d", 8);
	ADD_TEST_OK(on_fn, "d", 8);
	ADD_TEST_OK(on_fn_calling_game_fn_nothing, "d", 8);
	ADD_TEST_OK(on_fn_calling_game_fn_nothing_twice, "d", 8);
	ADD_TEST_OK(on_fn_calling_game_fn_plt_order, "d", 8);
	ADD_TEST_OK(on_fn_calling_helper_fns, "d", 8);
	ADD_TEST_OK(on_fn_overwriting_param, "s", 8);
	ADD_TEST_OK(on_fn_passing_argument_to_helper_fn, "d", 8);
	ADD_TEST_OK(on_fn_passing_magic_to_initialize, "d", 8);
	ADD_TEST_OK(on_fn_three, "j", 8);
	ADD_TEST_OK(on_fn_three_unused_first, "j", 8);
	ADD_TEST_OK(on_fn_three_unused_second, "j", 8);
	ADD_TEST_OK(on_fn_three_unused_third, "j", 8);
	ADD_TEST_OK(on_fn_usage_is_optional, "d", 8);
	ADD_TEST_OK(or_false, "d", 8);
	ADD_TEST_OK(or_short_circuit, "d", 8);
	ADD_TEST_OK(or_true_1, "d", 8);
	ADD_TEST_OK(or_true_2, "d", 8);
	ADD_TEST_OK(or_true_3, "d", 8);
	ADD_TEST_OK(pass_string_argument_to_game_fn, "d", 8);
	ADD_TEST_OK(pass_string_argument_to_helper_fn, "d", 8);
	ADD_TEST_OK(remainder_negative_result, "d", 8);
	ADD_TEST_OK(remainder_positive_result, "d", 8);
	ADD_TEST_OK(resource_and_entity, "b2", 8);
	ADD_TEST_OK(resource_and_entity_and_on_fn, "a2", 8);
	ADD_TEST_OK(resource_and_on_fn, "w", 8);
	ADD_TEST_OK(resource_can_contain_dot_1, "u", 8);
	ADD_TEST_OK(resource_can_contain_dot_2, "u", 8);
	ADD_TEST_OK(resource_can_contain_dot_3, "u", 8);
	ADD_TEST_OK(resource_can_contain_dot_dot_1, "u", 8);
	ADD_TEST_OK(resource_can_contain_dot_dot_2, "u", 8);
	ADD_TEST_OK(resource_can_contain_dot_dot_3, "u", 8);
	ADD_TEST_OK(resource_duplicate, "v", 8);
	ADD_TEST_OK(resource_in_define, "u", 8);
	ADD_TEST_OK(resource_in_define_and_entity_in_game_fn, "w", 8);
	ADD_TEST_OK(resource_twice, "v", 8);
	ADD_TEST_OK(return, "d", 8);
	ADD_TEST_OK(return_from_on_fn, "d", 8);
	ADD_TEST_OK(return_from_on_fn_minimal, "d", 8);
	ADD_TEST_OK(return_with_no_value, "d", 8);
	ADD_TEST_OK(same_variable_name_in_different_functions, "e", 8);
	ADD_TEST_OK(stack_16_byte_alignment, "d", 8);
	ADD_TEST_OK(stack_16_byte_alignment_midway, "d", 8);
	ADD_TEST_OK(spill_args_to_define_fn, "c2", 8);
	ADD_TEST_OK(spill_args_to_define_fn_subless, "d2", 8);
	ADD_TEST_OK(spill_args_to_game_fn, "d", 8);
	ADD_TEST_OK(spill_args_to_game_fn_subless, "d", 8);
	ADD_TEST_OK(spill_args_to_helper_fn, "d", 12);
	ADD_TEST_OK(spill_args_to_helper_fn_32_bit_f32, "d", 12);
	ADD_TEST_OK(spill_args_to_helper_fn_32_bit_i32, "d", 12);
	ADD_TEST_OK(spill_args_to_helper_fn_32_bit_string, "d", 12);
	ADD_TEST_OK(spill_args_to_helper_fn_subless, "d", 12);
	ADD_TEST_OK(string_and_on_fn, "p", 8);
	ADD_TEST_OK(string_can_be_passed_to_helper_fn, "d", 8);
	ADD_TEST_OK(string_eq_false, "d", 8);
	ADD_TEST_OK(string_eq_true, "d", 8);
	ADD_TEST_OK(string_eq_true_empty, "d", 8);
	ADD_TEST_OK(string_ne_false, "d", 8);
	ADD_TEST_OK(string_ne_false_empty, "d", 8);
	ADD_TEST_OK(string_ne_true, "d", 8);
	ADD_TEST_OK(sub_rsp_32_bits_global_variables_i32, "d", 268);
	ADD_TEST_OK(sub_rsp_32_bits_global_variables_id, "d", 528);
	ADD_TEST_OK(sub_rsp_32_bits_local_variables_i32, "d", 8);
	ADD_TEST_OK(sub_rsp_32_bits_local_variables_id, "d", 8);
	ADD_TEST_OK(subtraction_negative_result, "d", 8);
	ADD_TEST_OK(subtraction_positive_result, "d", 8);
	ADD_TEST_OK(variable, "d", 8);
	ADD_TEST_OK(variable_does_not_shadow_define_fn, "d", 8);
	ADD_TEST_OK(variable_reassignment, "d", 8);
	ADD_TEST_OK(variable_shadows_define_fn, "d", 8);
	ADD_TEST_OK(variable_shadows_game_fn, "d", 8);
	ADD_TEST_OK(variable_shadows_helper_fn, "d", 8);
	ADD_TEST_OK(variable_shadows_on_fn_1, "d", 8);
	ADD_TEST_OK(variable_shadows_on_fn_2, "j", 8);
	ADD_TEST_OK(variable_string_global, "d", 16);
	ADD_TEST_OK(variable_string_local, "d", 8);
	ADD_TEST_OK(void_function_early_return, "d", 8);
	ADD_TEST_OK(while_false, "d", 8);
	ADD_TEST_OK(write_to_global_variable, "d", 16);
}

int main(int argc, char *argv[]) {
	grug_init(runtime_error_handler, "mod_api.json", "tests");

#ifdef SHUFFLING
		// If a test failed, you can reproduce it
		// by replacing `time(NULL)` with the failing test's printed seed
		unsigned int seed = time(NULL);
		printf("The seed is %u\n", seed);
		srand(seed);
#endif

	for (int i = 1; i < argc; i++) {
		whitelisted_tests[whitelisted_tests_size++] = argv[i];
	}

	add_error_tests();
#ifdef SHUFFLING
		SHUFFLE(error_test_datas, error_test_datas_size, struct error_test_data);
#endif
	for (size_t i = 0; i < error_test_datas_size; i++) {
		struct error_test_data data = error_test_datas[i];

		test_error(
			data.test_name_str,
			data.grug_path,
			data.expected_error_path,
			data.results_path,
			data.output_dll_path,
			data.grug_output_path,
			data.failed_file_path
		);
	}

	add_runtime_error_tests();
#ifdef SHUFFLING
		SHUFFLE(runtime_error_test_datas, runtime_error_test_datas_size, struct runtime_error_test_data);
#endif
	for (size_t i = 0; i < runtime_error_test_datas_size; i++) {
		struct runtime_error_test_data fn_data = runtime_error_test_datas[i];

		struct test_data data = runtime_error_prologue(
			fn_data.test_name_str,
			fn_data.grug_path,
			fn_data.nasm_path,
			fn_data.expected_error_path,
			fn_data.results_path,
			fn_data.output_dll_path,
			fn_data.expected_dll_path,
			fn_data.nasm_o_path,
			fn_data.expected_xxd_path,
			fn_data.expected_readelf_path,
			fn_data.expected_objdump_path,
			fn_data.failed_file_path,
			fn_data.expected_define_type_str,
			fn_data.expected_globals_size_value
		);

		if (data.run) {
			runtime_error_reason = NULL;
			had_runtime_error = false;
			runtime_error_type = -1;
			runtime_error_on_fn_name = NULL;
			runtime_error_on_fn_path = NULL;

			fn_data.run(data.on_fns, data.g, data.resources_size, data.resources, data.entities_size, data.entities, data.entity_types);

			runtime_error_epilogue(
				fn_data.grug_path,
				fn_data.expected_error_path,
				fn_data.output_dll_path,
				fn_data.expected_dll_path,
				fn_data.output_xxd_path,
				fn_data.output_readelf_path,
				fn_data.output_objdump_path,
				fn_data.dump_path,
				fn_data.applied_path,
				fn_data.failed_file_path
			);
		}

		if (data.dll && dlclose(data.dll)) {
			handle_dlerror("dlclose");
		}
	}

	add_ok_tests();
#ifdef SHUFFLING
		SHUFFLE(ok_test_datas, ok_test_datas_size, struct ok_test_data);
#endif
	for (size_t i = 0; i < ok_test_datas_size; i++) {
		struct ok_test_data fn_data = ok_test_datas[i];

		struct test_data data = ok_prologue(
			fn_data.test_name_str,
			fn_data.grug_path,
			fn_data.nasm_path,
			fn_data.results_path,
			fn_data.output_dll_path,
			fn_data.expected_dll_path,
			fn_data.nasm_o_path,
			fn_data.expected_xxd_path,
			fn_data.expected_readelf_path,
			fn_data.expected_objdump_path,
			fn_data.failed_file_path,
			fn_data.expected_define_type_str,
			fn_data.expected_globals_size_value
		);

		if (data.run) {
			fn_data.run(data.on_fns, data.g, data.resources_size, data.resources, data.entities_size, data.entities, data.entity_types);

			ok_epilogue(
				fn_data.grug_path,
				fn_data.output_dll_path,
				fn_data.expected_dll_path,
				fn_data.output_xxd_path,
				fn_data.output_readelf_path,
				fn_data.output_objdump_path,
				fn_data.dump_path,
				fn_data.applied_path,
				fn_data.failed_file_path
			);
		}

		if (data.dll && dlclose(data.dll)) {
			handle_dlerror("dlclose");
		}
	}

	printf("\nAll tests passed! 🎉\n");
}
