#define _XOPEN_SOURCE 700

#include "grug/grug.h"

#include "mod_api.h"

#include <assert.h>
#include <dirent.h>
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
static size_t err_test_datas_size;

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
	size_t expected_globals_size_value;
};
static struct runtime_error_test_data runtime_error_test_datas[420420];
static size_t err_runtime_test_datas_size;

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
static size_t game_fn_get_evil_false_call_count;
static size_t game_fn_set_is_happy_call_count;
static size_t game_fn_mega_f32_call_count;
static size_t game_fn_mega_i32_call_count;
static size_t game_fn_draw_call_count;
static size_t game_fn_blocked_alrm_call_count;
static size_t game_fn_nothing_call_count;
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
static size_t game_fn_talk_call_count;
static size_t game_fn_get_position_call_count;
static size_t game_fn_cause_game_fn_error_call_count;
static size_t game_fn_call_on_b_fn_call_count;

static void (*on_b)(void *globals);
static void *g_for_on_b;

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
int game_fn_get_evil_false(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_get_evil_false_call_count++;

	return 0xff00;
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
static char *game_fn_draw_sprite_path;
void game_fn_draw(char *sprite_path) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_draw_call_count++;
	game_fn_draw_sprite_path = sprite_path;
}
void game_fn_blocked_alrm(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_blocked_alrm_call_count++;
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

	return 69;
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
static char *game_fn_talk_message1;
static char *game_fn_talk_message2;
static char *game_fn_talk_message3;
static char *game_fn_talk_message4;
void game_fn_talk(char *message1, char *message2, char *message3, char *message4) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_talk_call_count++;

	game_fn_talk_message1 = message1;
	game_fn_talk_message2 = message2;
	game_fn_talk_message3 = message3;
	game_fn_talk_message4 = message4;
}
static uint64_t game_fn_get_position_id;
uint64_t game_fn_get_position(uint64_t id) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_get_position_call_count++;

	game_fn_get_position_id = id;

	return 1337;
}
void game_fn_cause_game_fn_error(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_cause_game_fn_error_call_count++;

	grug_game_function_error_happened("cause_game_fn_error(): Game function error");
}
void game_fn_call_on_b_fn(void) {
	ASSERT_16_BYTE_STACK_ALIGNED();
	game_fn_call_on_b_fn_call_count++;

	on_b(g_for_on_b);
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
	game_fn_get_evil_false_call_count = 0;
	game_fn_set_is_happy_call_count = 0;
	game_fn_mega_f32_call_count = 0;
	game_fn_mega_i32_call_count = 0;
	game_fn_draw_call_count = 0;
	game_fn_blocked_alrm_call_count = 0;
	game_fn_nothing_call_count = 0;
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
	game_fn_talk_call_count = 0;
	game_fn_get_position_call_count = 0;
	game_fn_cause_game_fn_error_call_count = 0;
	game_fn_call_on_b_fn_call_count = 0;
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

#define ADD_TEST_ERROR(test_name, entity_type) {\
	if (whitelisted_tests_size == 0 || is_whitelisted_test(#test_name)) {\
		error_test_datas[err_test_datas_size++] = (struct error_test_data){\
			.test_name_str = #test_name,\
			.grug_path = "tests/err/"#test_name"/input-"entity_type".grug",\
			.expected_error_path = "tests/err/"#test_name"/expected_error.txt",\
			.results_path = "tests/err/"#test_name"/results",\
			.output_dll_path = "tests/err/"#test_name"/results/output.so",\
			.grug_output_path = "tests/err/"#test_name"/results/grug_output.txt",\
			.failed_file_path = "tests/err/"#test_name"/results/failed"\
		};\
	}\
}

#define ADD_TEST_RUNTIME_ERROR(test_name, entity_type, expected_globals_size) {\
	if (whitelisted_tests_size == 0 || is_whitelisted_test(#test_name)) {\
		runtime_error_test_datas[err_runtime_test_datas_size++] = (struct runtime_error_test_data){\
			.run = runtime_error_##test_name,\
			.test_name_str = #test_name,\
			.grug_path = "tests/err_runtime/"#test_name"/input-"entity_type".grug",\
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
			.expected_globals_size_value = expected_globals_size\
		};\
	}\
}

#define ADD_TEST_OK(test_name, entity_type, expected_globals_size) {\
	if (whitelisted_tests_size == 0 || is_whitelisted_test(#test_name)) {\
		ok_test_datas[ok_test_datas_size++] = (struct ok_test_data){\
			.run = ok_##test_name,\
			.test_name_str = #test_name,\
			.grug_path = "tests/ok/"#test_name"/input-"entity_type".grug",\
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
			.expected_globals_size_value = expected_globals_size\
		};\
	}\
}

// This is the Fisher-Yates shuffle:
// https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
// https://blog.codinghorror.com/the-danger-of-naivete/
#ifdef SHUFFLES
#define SHUFFLE(arr, size, T) {\
	for (int i = size; i > 0; i--) {\
		int n = rand() % i;\
		\
		T old = arr[i-1];\
		arr[i-1] = arr[n];\
		arr[n] = old;\
	}\
}
#endif

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

static bool shuffles_was_not_defined(void) {
#ifdef SHUFFLES
	return false;
#endif
	return true;
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
	 && shuffles_was_not_defined()
	 && newer(grug_output_path, grug_path)
	 && newer(grug_output_path, expected_error_path)
	 && newer(grug_output_path, "mod_api.json")
	 && newer(grug_output_path, "tests.sh")
	 && newer(grug_output_path, "tests.out")
	 && newer(grug_output_path, "tests/utils/defines.s")
	 && newer(grug_output_path, "tests/utils/macros.s")
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
		printf("Failed to dump file AST: %s: %s (detected by grug.c:%d)\n", grug_error.path, grug_error.msg, grug_error.grug_c_line_number);
		exit(EXIT_FAILURE);
	}

	if (grug_generate_file_from_json(dump_path, applied_path)) {
		printf("Failed to apply file AST: %s: %s (detected by grug.c:%d)\n", grug_error.path, grug_error.msg, grug_error.grug_c_line_number);
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

	// `-z noexecstack` is necessary in order for Arch Linux to not complain when dlopen()ing expected.so
	//   about a missing GNU_STACK program header.
	run((char *[]){"ld", nasm_o_path, "-o", expected_dll_path, "-x", "-shared", "--hash-style=sysv", "-z", "noexecstack", NULL});
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

	size_t globals_size = *(size_t *)get(dll, "globals_size");
	assert(globals_size == expected_globals_size);

	void *g = malloc(globals_size);

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	grug_init_globals_fn_t init_globals = get(dll, "init_globals");
	#pragma GCC diagnostic pop

	init_globals(g, 42);

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
	size_t expected_globals_size
) {
	if (failed_file_doesnt_exist(failed_file_path)
	 && shuffles_was_not_defined()
	 && newer(output_dll_path, nasm_path)
	 && newer(output_dll_path, grug_path)
	 && newer(output_dll_path, expected_error_path)
	 && newer(output_dll_path, expected_dll_path)
	 && newer(output_dll_path, "mod_api.json")
	 && newer(output_dll_path, "tests.sh")
	 && newer(output_dll_path, "tests.out")
	 && newer(output_dll_path, "tests/utils/defines.s")
	 && newer(output_dll_path, "tests/utils/macros.s")
	) {
		printf("Skipping tests/err_runtime/%s...\n", test_name);
		return (struct test_data){.run=false};
	}

	printf("Running tests/err_runtime/%s...\n", test_name);

	return get_expected_test_data(nasm_path, results_path, expected_dll_path, nasm_o_path, expected_xxd_path, expected_readelf_path, expected_objdump_path, failed_file_path, expected_globals_size);
}

static bool had_runtime_error = false;
static size_t error_handler_calls = 0;
static enum grug_runtime_error_type runtime_error_type = -1;
static char *runtime_error_on_fn_name = NULL;
static char *runtime_error_on_fn_path = NULL;
static void runtime_error_handler(char *reason, enum grug_runtime_error_type type, char *on_fn_name, char *on_fn_path) {
	had_runtime_error = true;
	error_handler_calls++;

	runtime_error_reason = reason;
	runtime_error_type = type;
	runtime_error_on_fn_name = on_fn_name;
	runtime_error_on_fn_path = on_fn_path;
}

static void runtime_error_all(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_DIVISION_BY_ZERO);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/all/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/all/input-d.grug"));

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

	assert(runtime_error_type == GRUG_ON_FN_DIVISION_BY_ZERO);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/division_by_0/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/division_by_0/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_game_fn_error(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_cause_game_fn_error_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_cause_game_fn_error_call_count == 1);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_GAME_FN_ERROR);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/game_fn_error/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/game_fn_error/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_game_fn_error_once(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_cause_game_fn_error_call_count == 0);
	((struct e_on_fns *)on_fns)->a(g);
	assert(game_fn_cause_game_fn_error_call_count == 1);

	assert(had_runtime_error);

	assert(runtime_error_type == GRUG_ON_FN_GAME_FN_ERROR);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/game_fn_error_once/input-e.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/game_fn_error_once/input-e.grug"));

	had_runtime_error = false;

	assert(game_fn_cause_game_fn_error_call_count == 1);
	assert(game_fn_nothing_call_count == 0);
	((struct e_on_fns *)on_fns)->b(g);
	assert(game_fn_cause_game_fn_error_call_count == 1);
	assert(game_fn_nothing_call_count == 1);

	assert(!had_runtime_error);

	free(g);

	assert(streq(grug_fn_name, "on_b"));
	assert(streq(grug_fn_path, "tests/err_runtime/game_fn_error_once/input-e.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_i32_overflow_addition(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/i32_overflow_addition/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/i32_overflow_addition/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_i32_overflow_division(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/i32_overflow_division/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/i32_overflow_division/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_i32_overflow_multiplication(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/i32_overflow_multiplication/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/i32_overflow_multiplication/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_i32_overflow_negation(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/i32_overflow_negation/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/i32_overflow_negation/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_i32_overflow_remainder(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/i32_overflow_remainder/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/i32_overflow_remainder/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_i32_overflow_subtraction(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/i32_overflow_subtraction/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/i32_overflow_subtraction/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_i32_underflow_addition(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/i32_underflow_addition/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/i32_underflow_addition/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_i32_underflow_multiplication(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/i32_underflow_multiplication/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/i32_underflow_multiplication/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_i32_underflow_subtraction(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/i32_underflow_subtraction/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/i32_underflow_subtraction/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_on_fn_calls_erroring_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	on_b = ((struct e_on_fns *)on_fns)->b;
	g_for_on_b = g;

	assert(game_fn_call_on_b_fn_call_count == 0);
	assert(game_fn_cause_game_fn_error_call_count == 0);
	assert(game_fn_nothing_call_count == 0);
	assert(error_handler_calls == 0);
	((struct e_on_fns *)on_fns)->a(g);
	assert(game_fn_call_on_b_fn_call_count == 1);
	assert(game_fn_cause_game_fn_error_call_count == 1);
	assert(game_fn_nothing_call_count == 0);
	assert(error_handler_calls == 2);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_GAME_FN_ERROR);

	assert(streq(runtime_error_on_fn_name, "on_b"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/on_fn_calls_erroring_on_fn/input-e.grug"));

	assert(streq(grug_fn_name, "on_b"));
	assert(streq(grug_fn_path, "tests/err_runtime/on_fn_calls_erroring_on_fn/input-e.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_on_fn_errors_after_it_calls_other_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	on_b = ((struct e_on_fns *)on_fns)->b;
	g_for_on_b = g;

	assert(game_fn_call_on_b_fn_call_count == 0);
	assert(game_fn_nothing_call_count == 0);
	assert(game_fn_cause_game_fn_error_call_count == 0);
	assert(error_handler_calls == 0);
	((struct e_on_fns *)on_fns)->a(g);
	assert(game_fn_call_on_b_fn_call_count == 1);
	assert(game_fn_nothing_call_count == 1);
	assert(game_fn_cause_game_fn_error_call_count == 1);
	assert(error_handler_calls == 1);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_GAME_FN_ERROR);

	assert(streq(runtime_error_on_fn_name, "on_b"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/on_fn_errors_after_it_calls_other_on_fn/input-e.grug"));

	assert(streq(grug_fn_name, "on_b"));
	assert(streq(grug_fn_path, "tests/err_runtime/on_fn_errors_after_it_calls_other_on_fn/input-e.grug"));

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

	assert(runtime_error_type == GRUG_ON_FN_DIVISION_BY_ZERO);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/remainder_by_0/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/remainder_by_0/input-d.grug"));

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

	assert(runtime_error_type == GRUG_ON_FN_STACK_OVERFLOW);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/stack_overflow/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/stack_overflow/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_time_limit_exceeded(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_TIME_LIMIT_EXCEEDED);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/time_limit_exceeded/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/time_limit_exceeded/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_time_limit_exceeded_exponential_calls(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_TIME_LIMIT_EXCEEDED);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/time_limit_exceeded_exponential_calls/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/time_limit_exceeded_exponential_calls/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void runtime_error_time_limit_exceeded_fibonacci(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	assert(had_runtime_error);

	free(g);

	assert(runtime_error_type == GRUG_ON_FN_TIME_LIMIT_EXCEEDED);

	assert(streq(runtime_error_on_fn_name, "on_a"));
	assert(streq(runtime_error_on_fn_path, "tests/err_runtime/time_limit_exceeded_fibonacci/input-d.grug"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/err_runtime/time_limit_exceeded_fibonacci/input-d.grug"));

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
	size_t expected_globals_size
) {
	if (failed_file_doesnt_exist(failed_file_path)
	 && shuffles_was_not_defined()
	 && newer(output_dll_path, nasm_path)
	 && newer(output_dll_path, grug_path)
	 && newer(output_dll_path, expected_dll_path)
	 && newer(output_dll_path, "mod_api.json")
	 && newer(output_dll_path, "tests.sh")
	 && newer(output_dll_path, "tests.out")
	 && newer(output_dll_path, "tests/utils/defines.s")
	 && newer(output_dll_path, "tests/utils/macros.s")
	) {
		printf("Skipping tests/ok/%s...\n", test_name);
		return (struct test_data){.run=false};
	}

	printf("Running tests/ok/%s...\n", test_name);

	return get_expected_test_data(nasm_path, results_path, expected_dll_path, nasm_o_path, expected_xxd_path, expected_readelf_path, expected_objdump_path, failed_file_path, expected_globals_size);
}

static void ok_addition_as_argument(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 3);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/addition_as_argument/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/addition_as_two_arguments/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/addition_with_multiplication/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/addition_with_multiplication_2/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/and_false_1/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/and_false_2/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/and_false_3/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/and_short_circuit/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/and_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/blocked_alrm/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/bool_logical_not_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/bool_logical_not_true/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_bool_returned(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_is_happy_call_count == 0);
	assert(game_fn_get_evil_false_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_is_happy_call_count == 1);
	assert(game_fn_get_evil_false_call_count == 1);

	free(g);

	assert(game_fn_set_is_happy_is_happy == false);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/bool_returned/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_bool_returned_global(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_is_happy_call_count == 0);
	assert(game_fn_get_evil_false_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_is_happy_call_count == 1);
	assert(game_fn_get_evil_false_call_count == 1);

	free(g);

	assert(game_fn_set_is_happy_is_happy == false);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/bool_returned_global/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_bool_zero_extended_if_statement(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/bool_zero_extended_if_statement/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_bool_zero_extended_while_statement(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/bool_zero_extended_while_statement/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/break/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/calls_100/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/calls_1000/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_calls_in_call(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_max_call_count == 0);
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_max_call_count == 3);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_max_x == 2);
	assert(game_fn_max_y == 4);
	assert(game_fn_initialize_x == 4);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/calls_in_call/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/comment_above_block/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/comment_above_block_twice/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/comment_above_helper_fn/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_above_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/comment_above_on_fn/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/comment_between_statements/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/comment_lone_block/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_lone_block_at_end(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/comment_lone_block_at_end/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_comment_lone_global(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/comment_lone_global/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/continue/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/division_negative_result/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/division_positive_result/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_double_negation_with_parentheses(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 2);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/double_negation_with_parentheses/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_double_not_with_parentheses(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/double_not_with_parentheses/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/else_after_else_if_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/else_after_else_if_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/else_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/else_if_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/else_if_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/else_true/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_empty_file(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/empty_line/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/entity_and_resource_as_subexpression/input-d.grug"));

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/entity_and_resource_as_subexpression/foo.txt"));

	assert(entities_size == 1);
	assert(streq(entities[0], "ok:baz"));
	assert(streq(entity_types[0], ""));
}

static void ok_entity_duplicate(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_spawn_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_spawn_call_count == 4);

	free(g);

	assert(streq(game_fn_spawn_name, "ok:baz"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/entity_duplicate/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 4);
	assert(streq(entities[0], "ok:foo"));
	assert(streq(entity_types[0], ""));
	assert(streq(entities[1], "ok:bar"));
	assert(streq(entity_types[1], ""));
	assert(streq(entities[2], "ok:bar"));
	assert(streq(entity_types[2], ""));
	assert(streq(entities[3], "ok:baz"));
	assert(streq(entity_types[3], ""));
}

static void ok_entity_in_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_spawn_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_spawn_call_count == 1);

	free(g);

	assert(streq(game_fn_spawn_name, "ok:foo"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/entity_in_on_fn/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 1);
	assert(streq(entities[0], "ok:foo"));
	assert(streq(entity_types[0], ""));
}

static void ok_entity_in_on_fn_with_mod_specified(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_spawn_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_spawn_call_count == 1);

	free(g);

	assert(streq(game_fn_spawn_name, "wow:foo"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/entity_in_on_fn_with_mod_specified/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 1);
	assert(streq(entities[0], "wow:foo"));
	assert(streq(entity_types[0], ""));
}

static void ok_eq_false(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == false);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/eq_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/eq_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_addition/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_argument/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_division/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_eq_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_eq_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_ge_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_ge_true_1/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_ge_true_2/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_global_variable/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_gt_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_gt_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_le_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_le_true_1/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_le_true_2/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_local_variable/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_lt_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_lt_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_multiplication/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_ne_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_negated/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_ne_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_passed_to_helper_fn/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_passed_to_on_fn/input-r.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_passing_sin_to_cos/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/f32_subtraction/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/fibonacci/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/ge_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/ge_true_1/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/ge_true_2/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_global_2_does_not_have_error_handling(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	assert(((int32_t*)globals)[0] == -1);
	assert(((int32_t*)globals)[1] == 0);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_global_call_using_me(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_get_position_call_count == 1);

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	assert(((uint64_t*)globals)[0] == 1337);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_global_call_using_null_id(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_get_position_call_count == 1);

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	assert(((uint64_t*)globals)[0] == 1337);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_global_can_use_earlier_global(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 5);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/global_can_use_earlier_global/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_global_containing_negation(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	assert(((int32_t*)globals)[0] == -2);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_global_id(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	assert(game_fn_get_opponent_call_count == 1);

	char *globals = g;
	assert(*((uint64_t*)globals) == 42);
	globals += sizeof(uint64_t);

	assert(((uint64_t*)globals)[0] == 69);

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_globals(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/gt_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/gt_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/helper_fn/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/helper_fn_called_in_if/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/helper_fn_called_indirectly/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/helper_fn_overwriting_param/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/helper_fn_returning_void_has_no_return/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/helper_fn_returning_void_returns_void/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_helper_fn_same_param_name_as_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/helper_fn_same_param_name_as_on_fn/input-f.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_helper_fn_same_param_name_as_other_helper_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 2);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/helper_fn_same_param_name_as_other_helper_fn/input-f.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/i32_max/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/i32_min/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/i32_negated/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_i32_negative_is_smaller_than_positive(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_bool_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_bool_call_count == 1);

	free(g);

	assert(game_fn_initialize_bool_b == true);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/i32_negative_is_smaller_than_positive/input-d.grug"));

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

	assert(game_fn_set_target_target == 69);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/id_local_variable_get_and_set/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/if_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/if_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/le_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/le_true_1/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/le_true_2/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_local_id_can_be_reassigned(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_get_opponent_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_get_opponent_call_count == 2);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/local_id_can_be_reassigned/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/lt_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/lt_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/max_args/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/me/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/me_assigned_to_local_variable/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/me_passed_to_helper_fn/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_mov_32_bits_global_i32(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	free(g);

	assert(streq(grug_fn_name, "init_globals"));
	assert(streq(grug_fn_path, "tests/ok/mov_32_bits_global_i32/input-a.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_mov_32_bits_global_id(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	(void)on_fns;

	free(g);

	assert(streq(grug_fn_name, "init_globals"));
	assert(streq(grug_fn_path, "tests/ok/mov_32_bits_global_id/input-a.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/multiplication_as_two_arguments/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/ne_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/ne_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/negate_parenthesized_expr/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/negative_literal/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/nested_break/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/nested_continue/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/null_id_initializing_local/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/no_empty_line_between_statements/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_calling_game_fn_nothing/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_calling_game_fn_nothing_twice/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_calling_game_fn_plt_order/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_calling_helper_fns/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_calling_no_game_fn(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_calling_no_game_fn/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_calling_no_game_fn_but_with_addition(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_calling_no_game_fn_but_with_addition/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_calling_no_game_fn_but_with_global(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct d_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_calling_no_game_fn_but_with_global/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_overwriting_param/input-s.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_passing_argument_to_helper_fn/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_passing_magic_to_initialize/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_three(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);
	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_three/input-j.grug"));

	((struct j_on_fns *)on_fns)->b(g);
	assert(streq(grug_fn_name, "on_b"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_three/input-j.grug"));

	((struct j_on_fns *)on_fns)->c(g);
	assert(streq(grug_fn_name, "on_c"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_three/input-j.grug"));

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
	assert(streq(grug_fn_name, "on_b"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_three_unused_first/input-j.grug"));

	((struct j_on_fns *)on_fns)->c(g);
	assert(streq(grug_fn_name, "on_c"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_three_unused_first/input-j.grug"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_three_unused_second(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);
	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_three_unused_second/input-j.grug"));

	assert(((struct j_on_fns *)on_fns)->b == NULL);

	((struct j_on_fns *)on_fns)->c(g);
	assert(streq(grug_fn_name, "on_c"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_three_unused_second/input-j.grug"));

	free(g);

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_on_fn_three_unused_third(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);
	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_three_unused_third/input-j.grug"));

	((struct j_on_fns *)on_fns)->b(g);
	assert(streq(grug_fn_name, "on_b"));
	assert(streq(grug_fn_path, "tests/ok/on_fn_three_unused_third/input-j.grug"));

	assert(((struct j_on_fns *)on_fns)->c == NULL);

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/or_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/or_short_circuit/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/or_true_1/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/or_true_2/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/or_true_3/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/pass_string_argument_to_game_fn/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/pass_string_argument_to_helper_fn/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_remainder_negative_negative(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -1);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/remainder_negative_negative/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_remainder_negative_positive(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == -1);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/remainder_negative_positive/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_remainder_positive_negative(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 1);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/remainder_positive_negative/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_remainder_positive_positive(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 1);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/remainder_positive_positive/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_and_entity(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_draw_call_count == 0);
	assert(game_fn_spawn_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_draw_call_count == 1);
	assert(game_fn_spawn_call_count == 1);

	free(g);

	assert(streq(game_fn_draw_sprite_path, "tests/ok/resource_and_entity/foo.txt"));
	assert(streq(game_fn_spawn_name, "ok:foo"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/resource_and_entity/input-d.grug"));

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_and_entity/foo.txt"));

	assert(entities_size == 1);
	assert(streq(entities[0], "ok:foo"));
	assert(streq(entity_types[0], ""));
}

static void ok_resource_can_contain_dot_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_draw_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_draw_call_count == 1);

	free(g);

	assert(streq(game_fn_draw_sprite_path, "tests/ok/resource_can_contain_dot_1/.foo"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/resource_can_contain_dot_1/input-d.grug"));

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_1/.foo"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_draw_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_draw_call_count == 1);

	free(g);

	assert(streq(game_fn_draw_sprite_path, "tests/ok/resource_can_contain_dot_2/foo."));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/resource_can_contain_dot_2/input-d.grug"));

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_2/foo."));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_3(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_draw_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_draw_call_count == 1);

	free(g);

	assert(streq(game_fn_draw_sprite_path, "tests/ok/resource_can_contain_dot_3/foo.bar"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/resource_can_contain_dot_3/input-d.grug"));

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_3/foo.bar"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_dot_1(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_draw_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_draw_call_count == 1);

	free(g);

	assert(streq(game_fn_draw_sprite_path, "tests/ok/resource_can_contain_dot_dot_1/..foo"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/resource_can_contain_dot_dot_1/input-d.grug"));

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_dot_1/..foo"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_dot_2(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_draw_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_draw_call_count == 1);

	free(g);

	assert(streq(game_fn_draw_sprite_path, "tests/ok/resource_can_contain_dot_dot_2/foo.."));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/resource_can_contain_dot_dot_2/input-d.grug"));

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_dot_2/foo.."));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_can_contain_dot_dot_3(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_draw_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_draw_call_count == 1);

	free(g);

	assert(streq(game_fn_draw_sprite_path, "tests/ok/resource_can_contain_dot_dot_3/foo..bar"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/resource_can_contain_dot_dot_3/input-d.grug"));

	assert(resources_size == 1);
	assert(streq(resources[0], "tests/ok/resource_can_contain_dot_dot_3/foo..bar"));

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_resource_duplicate(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_draw_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_draw_call_count == 4);

	free(g);

	assert(streq(game_fn_draw_sprite_path, "tests/ok/resource_duplicate/baz.txt"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/resource_duplicate/input-d.grug"));

	assert(resources_size == 3);
	assert(streq(resources[0], "tests/ok/resource_duplicate/foo.txt"));
	assert(streq(resources[1], "tests/ok/resource_duplicate/bar.txt"));
	assert(streq(resources[2], "tests/ok/resource_duplicate/baz.txt"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/return/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/return_from_on_fn/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_return_from_on_fn_minimal(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	((struct j_on_fns *)on_fns)->a(g);

	free(g);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/return_from_on_fn_minimal/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/return_with_no_value/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/same_variable_name_in_different_functions/input-e.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/spill_args_to_game_fn/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/spill_args_to_game_fn_subless/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/spill_args_to_helper_fn/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/spill_args_to_helper_fn_32_bit_f32/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/spill_args_to_helper_fn_32_bit_i32/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/spill_args_to_helper_fn_32_bit_string/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/spill_args_to_helper_fn_subless/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_stack_16_byte_alignment(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_nothing_call_count == 0);
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_nothing_call_count == 1);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/stack_16_byte_alignment/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_stack_16_byte_alignment_midway(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_magic_call_count == 0);
	assert(game_fn_initialize_call_count == 0);
	((struct j_on_fns *)on_fns)->a(g);
	assert(game_fn_magic_call_count == 1);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 42 + 42);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/stack_16_byte_alignment_midway/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/string_can_be_passed_to_helper_fn/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_string_duplicate(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_talk_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_talk_call_count == 1);

	free(g);

	assert(streq(game_fn_talk_message1, "foo"));
	assert(streq(game_fn_talk_message2, "bar"));
	assert(streq(game_fn_talk_message3, "bar"));
	assert(streq(game_fn_talk_message4, "baz"));

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/string_duplicate/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/string_eq_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/string_eq_true/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/string_eq_true_empty/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/string_ne_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/string_ne_false_empty/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/string_ne_true/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_sub_rsp_32_bits_local_variables_i32(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 30);

	free(g);

	assert(game_fn_initialize_x == 30);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/sub_rsp_32_bits_local_variables_i32/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_sub_rsp_32_bits_local_variables_id(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_set_target_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_set_target_call_count == 15);

	free(g);

	assert(game_fn_set_target_target == UINT64_MAX);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/sub_rsp_32_bits_local_variables_id/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/subtraction_negative_result/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/subtraction_positive_result/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/variable/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_does_not_shadow_in_different_if_statement(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 2);

	free(g);

	assert(game_fn_initialize_x == 69);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/variable_does_not_shadow_in_different_if_statement/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/variable_reassignment/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

static void ok_variable_reassignment_does_not_dealloc_outer_variable(void *on_fns, void *g, size_t resources_size, char **resources, size_t entities_size, char **entities, char **entity_types) {
	assert(game_fn_initialize_call_count == 0);
	((struct d_on_fns *)on_fns)->a(g);
	assert(game_fn_initialize_call_count == 1);

	free(g);

	assert(game_fn_initialize_x == 69);

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/variable_reassignment_does_not_dealloc_outer_variable/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/variable_string_global/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/variable_string_local/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/void_function_early_return/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/while_false/input-d.grug"));

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

	assert(streq(grug_fn_name, "on_a"));
	assert(streq(grug_fn_path, "tests/ok/write_to_global_variable/input-d.grug"));

	assert(resources_size == 0);
	assert(resources == NULL);

	assert(entities_size == 0);
	assert(entities == NULL);
	assert(entity_types == NULL);
}

#define CHECK_THAT_EVERY_TEST_DIRECTORY_HAS_A_FUNCTION(test_dirname) {\
	size_t entries = 0;\
	\
	DIR *dirp = opendir("tests/" #test_dirname);\
	assert(dirp);\
	\
	struct dirent *dp;\
	while ((dp = readdir(dirp))) {\
		if (streq(dp->d_name, ".") || streq(dp->d_name, "..")) {\
			continue;\
		}\
		entries++;\
	}\
	\
	if (entries != test_dirname ## _test_datas_size) {\
		fprintf(stderr, "The tests/" #test_dirname "/ directory contains %zu entries, which doesn't match it having %zu test functions\n", entries, test_dirname ## _test_datas_size);\
		exit(EXIT_FAILURE);\
	}\
	\
	assert(closedir(dirp) == 0);\
}

static void add_error_tests(void) {
	ADD_TEST_ERROR(assignment_isnt_expression, "d");
	ADD_TEST_ERROR(bool_cant_be_initialized_with_1, "d");
	ADD_TEST_ERROR(bool_ge, "d");
	ADD_TEST_ERROR(bool_gt, "d");
	ADD_TEST_ERROR(bool_le, "d");
	ADD_TEST_ERROR(bool_lt, "d");
	ADD_TEST_ERROR(bool_unary_minus, "d");
	ADD_TEST_ERROR(cant_add_strings, "d");
	ADD_TEST_ERROR(cant_break_outside_of_loop, "d");
	ADD_TEST_ERROR(cant_continue_outside_of_loop, "d");
	ADD_TEST_ERROR(cant_declare_me_globally, "a");
	ADD_TEST_ERROR(cant_declare_me_locally, "d");
	ADD_TEST_ERROR(cant_declare_variable_in_fn_call, "d");
	ADD_TEST_ERROR(cant_redefine_global, "d");
	ADD_TEST_ERROR(comment_at_the_end_of_another_statement, "d");
	ADD_TEST_ERROR(comment_lone_global_at_end, "a");
	ADD_TEST_ERROR(double_negation, "d");
	ADD_TEST_ERROR(double_not, "d");
	ADD_TEST_ERROR(empty_helper_fn, "d");
	ADD_TEST_ERROR(empty_line_after_group, "d");
	ADD_TEST_ERROR(empty_line_at_start_of_file, "d");
	ADD_TEST_ERROR(empty_line_before_group, "d");
	ADD_TEST_ERROR(empty_line_fn_group, "d");
	ADD_TEST_ERROR(empty_line_twice_at_end_of_file, "d");
	ADD_TEST_ERROR(empty_line_twice_between_local_statements, "d");
	ADD_TEST_ERROR(empty_line_while_group, "d");
	ADD_TEST_ERROR(empty_on_fn, "d");
	ADD_TEST_ERROR(entity_cant_be_empty_string, "d");
	ADD_TEST_ERROR(entity_cant_be_passed_to_helper_fn, "d");
	ADD_TEST_ERROR(entity_has_invalid_entity_name_colon, "d");
	ADD_TEST_ERROR(entity_has_invalid_entity_name_uppercase, "d");
	ADD_TEST_ERROR(entity_has_invalid_mod_name_uppercase, "d");
	ADD_TEST_ERROR(entity_mod_name_and_entity_name_is_missing, "d");
	ADD_TEST_ERROR(entity_mod_name_cant_be_current_mod, "d");
	ADD_TEST_ERROR(entity_mod_name_is_missing, "d");
	ADD_TEST_ERROR(entity_name_is_missing, "d");
	ADD_TEST_ERROR(f32_missing_digit_after_decimal_point, "d");
	ADD_TEST_ERROR(f32_too_big, "d");
	ADD_TEST_ERROR(f32_too_close_to_zero_negative, "d");
	ADD_TEST_ERROR(f32_too_close_to_zero_positive, "d");
	ADD_TEST_ERROR(f32_too_small, "d");
	ADD_TEST_ERROR(game_fn_call_gets_void, "d");
	ADD_TEST_ERROR(game_fn_does_not_exist, "d");
	ADD_TEST_ERROR(game_function_call_gets_wrong_arg_type, "d");
	ADD_TEST_ERROR(game_function_call_less_args_expected, "d");
	ADD_TEST_ERROR(game_function_call_more_args_expected, "d");
	ADD_TEST_ERROR(game_function_call_no_args_expected, "d");
	ADD_TEST_ERROR(global_cant_be_me, "a");
	ADD_TEST_ERROR(global_cant_be_null_id, "a");
	ADD_TEST_ERROR(global_cant_call_helper_fn, "a");
	ADD_TEST_ERROR(global_cant_call_on_fn, "d");
	ADD_TEST_ERROR(global_cant_use_later_global, "a");
	ADD_TEST_ERROR(global_id_cant_be_reassigned, "d");
	ADD_TEST_ERROR(global_variable_after_on_fns, "d");
	ADD_TEST_ERROR(global_variable_already_uses_local_variable_name, "d");
	ADD_TEST_ERROR(global_variable_contains_double_negation, "a");
	ADD_TEST_ERROR(global_variable_contains_double_not, "a");
	ADD_TEST_ERROR(global_variable_contains_entity, "a");
	ADD_TEST_ERROR(global_variable_contains_resource, "a");
	ADD_TEST_ERROR(global_variable_definition_cant_use_itself, "a");
	ADD_TEST_ERROR(global_variable_definition_missing_type, "a");
	ADD_TEST_ERROR(global_variable_definition_requires_value_i32, "d");
	ADD_TEST_ERROR(global_variable_definition_requires_value_string, "d");
	ADD_TEST_ERROR(helper_fn_call_gets_void, "d");
	ADD_TEST_ERROR(helper_fn_call_gets_wrong_arg_type, "d");
	ADD_TEST_ERROR(helper_fn_call_less_args_expected, "d");
	ADD_TEST_ERROR(helper_fn_call_more_args_expected, "d");
	ADD_TEST_ERROR(helper_fn_call_no_args_expected, "d");
	ADD_TEST_ERROR(helper_fn_defined_before_first_helper_fn_usage, "d");
	ADD_TEST_ERROR(helper_fn_defined_between_on_fns, "e");
	ADD_TEST_ERROR(helper_fn_different_return_value_expected, "d");
	ADD_TEST_ERROR(helper_fn_does_not_exist, "d");
	ADD_TEST_ERROR(helper_fn_duplicate, "d");
	ADD_TEST_ERROR(helper_fn_is_not_called_1, "d");
	ADD_TEST_ERROR(helper_fn_is_not_called_2, "d");
	ADD_TEST_ERROR(helper_fn_is_not_called_3, "d");
	ADD_TEST_ERROR(helper_fn_is_not_called_4, "d");
	ADD_TEST_ERROR(helper_fn_is_not_called_5, "d");
	ADD_TEST_ERROR(helper_fn_missing_return_statement, "d");
	ADD_TEST_ERROR(helper_fn_no_return_value_expected, "d");
	ADD_TEST_ERROR(helper_fn_return_with_comment_after_it, "d");
	ADD_TEST_ERROR(i32_logical_not, "d");
	ADD_TEST_ERROR(i32_too_big, "d");
	ADD_TEST_ERROR(i32_too_small, "d");
	ADD_TEST_ERROR(indentation_going_down_by_2, "d");
	ADD_TEST_ERROR(indented_call_argument, "d");
	ADD_TEST_ERROR(indented_call_arguments, "d");
	ADD_TEST_ERROR(indented_helper_fn_parameter, "d");
	ADD_TEST_ERROR(indented_helper_fn_parameters, "d");
	ADD_TEST_ERROR(indented_on_fn_parameter, "f");
	ADD_TEST_ERROR(indented_on_fn_parameters, "g");
	ADD_TEST_ERROR(local_variable_already_exists, "d");
	ADD_TEST_ERROR(local_variable_definition_cant_use_itself, "d");
	ADD_TEST_ERROR(local_variable_definition_missing_type, "d");
	ADD_TEST_ERROR(max_expr_recursion_depth_exceeded, "d");
	ADD_TEST_ERROR(max_statement_recursion_depth_exceeded, "d");
	ADD_TEST_ERROR(me_cant_be_written_to, "d");
	ADD_TEST_ERROR(me_plus_1, "d");
	ADD_TEST_ERROR(me_plus_me, "d");
	ADD_TEST_ERROR(missing_empty_line_between_global_and_on_fn, "d");
	ADD_TEST_ERROR(missing_empty_line_between_on_fn_and_helper_fn, "d");
	ADD_TEST_ERROR(newline_statement, "d");
	ADD_TEST_ERROR(no_space_between_comment_character_and_comment, "d");
	ADD_TEST_ERROR(not_followed_by_negation, "d");
	ADD_TEST_ERROR(null_id_plus_1, "d");
	ADD_TEST_ERROR(null_id_plus_null_id, "d");
	ADD_TEST_ERROR(on_fn_cant_be_called_by_helper_fn, "d");
	ADD_TEST_ERROR(on_fn_cant_be_called_by_on_fn, "d");
	ADD_TEST_ERROR(on_fn_defined_after_helper_fn, "d");
	ADD_TEST_ERROR(on_fn_duplicate, "d");
	ADD_TEST_ERROR(on_fn_was_not_declared_in_entity, "a");
	ADD_TEST_ERROR(on_fn_wrong_order, "e");
	ADD_TEST_ERROR(on_function_gets_wrong_arg_name, "f");
	ADD_TEST_ERROR(on_function_gets_wrong_arg_type, "f");
	ADD_TEST_ERROR(on_function_less_args_expected, "f");
	ADD_TEST_ERROR(on_function_more_args_expected, "g");
	ADD_TEST_ERROR(on_function_no_args_expected, "d");
	ADD_TEST_ERROR(on_function_no_return_value_expected, "d");
	ADD_TEST_ERROR(pass_bool_to_i32_game_param, "d");
	ADD_TEST_ERROR(pass_bool_to_i32_helper_param, "d");
	ADD_TEST_ERROR(remainder_by_float, "d");
	ADD_TEST_ERROR(resource_cant_be_empty_string, "d");
	ADD_TEST_ERROR(resource_cant_be_passed_to_helper_fn, "d");
	ADD_TEST_ERROR(resource_cant_contain_backslash, "d");
	ADD_TEST_ERROR(resource_cant_contain_double_slash, "d");
	ADD_TEST_ERROR(resource_cant_go_up_to_parent_directory_1, "d");
	ADD_TEST_ERROR(resource_cant_go_up_to_parent_directory_2, "d");
	ADD_TEST_ERROR(resource_cant_go_up_to_parent_directory_3, "d");
	ADD_TEST_ERROR(resource_cant_go_up_to_parent_directory_4, "d");
	ADD_TEST_ERROR(resource_cant_have_leading_slash, "d");
	ADD_TEST_ERROR(resource_cant_have_trailing_slash, "d");
	ADD_TEST_ERROR(resource_cant_refer_to_current_directory_1, "d");
	ADD_TEST_ERROR(resource_cant_refer_to_current_directory_2, "d");
	ADD_TEST_ERROR(resource_cant_refer_to_current_directory_3, "d");
	ADD_TEST_ERROR(resource_cant_refer_to_current_directory_4, "d");
	ADD_TEST_ERROR(resource_type_for_global, "a");
	ADD_TEST_ERROR(resource_type_for_helper_fn_argument, "d");
	ADD_TEST_ERROR(resource_type_for_helper_fn_return_type, "d");
	ADD_TEST_ERROR(resource_type_for_local, "d");
	ADD_TEST_ERROR(resource_type_for_on_fn_argument, "d");
	ADD_TEST_ERROR(string_pointer_arithmetic, "d");
	ADD_TEST_ERROR(trailing_space_in_comment, "d");
	ADD_TEST_ERROR(unary_plus_does_not_exist, "d");
	ADD_TEST_ERROR(unclosed_double_quote, "a");
	ADD_TEST_ERROR(unknown_variable, "d");
	ADD_TEST_ERROR(unused_expr_result, "d");
	ADD_TEST_ERROR(variable_assignment_before_definition, "d");
	ADD_TEST_ERROR(variable_definition_requires_value_i32, "d");
	ADD_TEST_ERROR(variable_definition_requires_value_string, "d");
	ADD_TEST_ERROR(variable_not_accessible, "d");
	ADD_TEST_ERROR(variable_same_name_missing_type, "d");
	ADD_TEST_ERROR(variable_shadows_argument, "f");
	ADD_TEST_ERROR(variable_shadows_global_variable, "d");
	ADD_TEST_ERROR(variable_shadows_local_variable, "d");
	ADD_TEST_ERROR(variable_statement_missing_assignment, "d");
	ADD_TEST_ERROR(variable_used_before_definition, "d");
	ADD_TEST_ERROR(wrong_type_global_assignment, "d");
	ADD_TEST_ERROR(wrong_type_global_reassignment, "d");
	ADD_TEST_ERROR(wrong_type_local_assignment, "d");
	ADD_TEST_ERROR(wrong_type_local_reassignment, "d");
}

static void add_runtime_error_tests(void) {
	ADD_TEST_RUNTIME_ERROR(all, "d", 8);
	ADD_TEST_RUNTIME_ERROR(division_by_0, "d", 8);
	ADD_TEST_RUNTIME_ERROR(game_fn_error, "d", 8);
	ADD_TEST_RUNTIME_ERROR(game_fn_error_once, "e", 8);
	ADD_TEST_RUNTIME_ERROR(i32_overflow_addition, "d", 8);
	ADD_TEST_RUNTIME_ERROR(i32_overflow_division, "d", 8);
	ADD_TEST_RUNTIME_ERROR(i32_overflow_multiplication, "d", 8);
	ADD_TEST_RUNTIME_ERROR(i32_overflow_negation, "d", 8);
	ADD_TEST_RUNTIME_ERROR(i32_overflow_remainder, "d", 8);
	ADD_TEST_RUNTIME_ERROR(i32_overflow_subtraction, "d", 8);
	ADD_TEST_RUNTIME_ERROR(i32_underflow_addition, "d", 8);
	ADD_TEST_RUNTIME_ERROR(i32_underflow_multiplication, "d", 8);
	ADD_TEST_RUNTIME_ERROR(i32_underflow_subtraction, "d", 8);
	ADD_TEST_RUNTIME_ERROR(on_fn_calls_erroring_on_fn, "e", 8);
	ADD_TEST_RUNTIME_ERROR(on_fn_errors_after_it_calls_other_on_fn, "e", 8);
	ADD_TEST_RUNTIME_ERROR(remainder_by_0, "d", 8);
	ADD_TEST_RUNTIME_ERROR(stack_overflow, "d", 8);
	ADD_TEST_RUNTIME_ERROR(time_limit_exceeded, "d", 8);
	ADD_TEST_RUNTIME_ERROR(time_limit_exceeded_exponential_calls, "d", 8);
	ADD_TEST_RUNTIME_ERROR(time_limit_exceeded_fibonacci, "d", 8);
}

static void add_ok_tests(void) {
	ADD_TEST_OK(addition_as_argument, "d", 8);
	ADD_TEST_OK(addition_as_two_arguments, "d", 8);
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
	ADD_TEST_OK(bool_returned_global, "d", 9);
	ADD_TEST_OK(bool_zero_extended_if_statement, "d", 8);
	ADD_TEST_OK(bool_zero_extended_while_statement, "d", 8);
	ADD_TEST_OK(break, "d", 8);
	ADD_TEST_OK(calls_100, "d", 8);
	ADD_TEST_OK(calls_1000, "d", 8);
	ADD_TEST_OK(calls_in_call, "d", 8);
	ADD_TEST_OK(comment_above_block, "d", 8);
	ADD_TEST_OK(comment_above_block_twice, "d", 8);
	ADD_TEST_OK(comment_above_globals, "a", 20);
	ADD_TEST_OK(comment_above_helper_fn, "d", 8);
	ADD_TEST_OK(comment_above_on_fn, "d", 8);
	ADD_TEST_OK(comment_between_globals, "a", 16);
	ADD_TEST_OK(comment_between_statements, "d", 8);
	ADD_TEST_OK(comment_lone_block, "d", 8);
	ADD_TEST_OK(comment_lone_block_at_end, "d", 8);
	ADD_TEST_OK(comment_lone_global, "d", 8);
	ADD_TEST_OK(continue, "d", 8);
	ADD_TEST_OK(division_negative_result, "d", 8);
	ADD_TEST_OK(division_positive_result, "d", 8);
	ADD_TEST_OK(double_negation_with_parentheses, "d", 8);
	ADD_TEST_OK(double_not_with_parentheses, "d", 8);
	ADD_TEST_OK(else_after_else_if_false, "d", 8);
	ADD_TEST_OK(else_after_else_if_true, "d", 8);
	ADD_TEST_OK(else_false, "d", 8);
	ADD_TEST_OK(else_if_false, "d", 8);
	ADD_TEST_OK(else_if_true, "d", 8);
	ADD_TEST_OK(else_true, "d", 8);
	ADD_TEST_OK(empty_file, "a", 8);
	ADD_TEST_OK(empty_line, "d", 8);
	ADD_TEST_OK(entity_and_resource_as_subexpression, "d", 8);
	ADD_TEST_OK(entity_duplicate, "d", 8);
	ADD_TEST_OK(entity_in_on_fn, "d", 8);
	ADD_TEST_OK(entity_in_on_fn_with_mod_specified, "d", 8);
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
	ADD_TEST_OK(global_2_does_not_have_error_handling, "a", 16);
	ADD_TEST_OK(global_call_using_me, "a", 16);
	ADD_TEST_OK(global_call_using_null_id, "a", 16);
	ADD_TEST_OK(global_can_use_earlier_global, "d", 16);
	ADD_TEST_OK(global_containing_negation, "a", 12);
	ADD_TEST_OK(global_id, "a", 16);
	ADD_TEST_OK(globals, "a", 16);
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
	ADD_TEST_OK(helper_fn_same_param_name_as_on_fn, "f", 8);
	ADD_TEST_OK(helper_fn_same_param_name_as_other_helper_fn, "f", 8);
	ADD_TEST_OK(i32_max, "d", 8);
	ADD_TEST_OK(i32_min, "d", 8);
	ADD_TEST_OK(i32_negated, "d", 8);
	ADD_TEST_OK(i32_negative_is_smaller_than_positive, "d", 8);
	ADD_TEST_OK(id_local_variable_get_and_set, "d", 8);
	ADD_TEST_OK(if_false, "d", 8);
	ADD_TEST_OK(if_true, "d", 8);
	ADD_TEST_OK(le_false, "d", 8);
	ADD_TEST_OK(le_true_1, "d", 8);
	ADD_TEST_OK(le_true_2, "d", 8);
	ADD_TEST_OK(local_id_can_be_reassigned, "d", 8);
	ADD_TEST_OK(lt_false, "d", 8);
	ADD_TEST_OK(lt_true, "d", 8);
	ADD_TEST_OK(max_args, "d", 8);
	ADD_TEST_OK(me, "d", 8);
	ADD_TEST_OK(me_assigned_to_local_variable, "d", 8);
	ADD_TEST_OK(me_passed_to_helper_fn, "d", 8);
	ADD_TEST_OK(mov_32_bits_global_i32, "a", 0x84);
	ADD_TEST_OK(mov_32_bits_global_id, "a", 0x88);
	ADD_TEST_OK(multiplication_as_two_arguments, "d", 8);
	ADD_TEST_OK(ne_false, "d", 8);
	ADD_TEST_OK(ne_true, "d", 8);
	ADD_TEST_OK(negate_parenthesized_expr, "d", 8);
	ADD_TEST_OK(negative_literal, "d", 8);
	ADD_TEST_OK(nested_break, "d", 8);
	ADD_TEST_OK(nested_continue, "d", 8);
	ADD_TEST_OK(null_id_initializing_local, "d", 8);
	ADD_TEST_OK(no_empty_line_between_globals, "a", 16);
	ADD_TEST_OK(no_empty_line_between_statements, "d", 8);
	ADD_TEST_OK(on_fn, "d", 8);
	ADD_TEST_OK(on_fn_calling_game_fn_nothing, "d", 8);
	ADD_TEST_OK(on_fn_calling_game_fn_nothing_twice, "d", 8);
	ADD_TEST_OK(on_fn_calling_game_fn_plt_order, "d", 8);
	ADD_TEST_OK(on_fn_calling_helper_fns, "d", 8);
	ADD_TEST_OK(on_fn_calling_no_game_fn, "d", 8);
	ADD_TEST_OK(on_fn_calling_no_game_fn_but_with_addition, "d", 8);
	ADD_TEST_OK(on_fn_calling_no_game_fn_but_with_global, "d", 12);
	ADD_TEST_OK(on_fn_overwriting_param, "s", 8);
	ADD_TEST_OK(on_fn_passing_argument_to_helper_fn, "d", 8);
	ADD_TEST_OK(on_fn_passing_magic_to_initialize, "d", 8);
	ADD_TEST_OK(on_fn_three, "j", 8);
	ADD_TEST_OK(on_fn_three_unused_first, "j", 8);
	ADD_TEST_OK(on_fn_three_unused_second, "j", 8);
	ADD_TEST_OK(on_fn_three_unused_third, "j", 8);
	ADD_TEST_OK(or_false, "d", 8);
	ADD_TEST_OK(or_short_circuit, "d", 8);
	ADD_TEST_OK(or_true_1, "d", 8);
	ADD_TEST_OK(or_true_2, "d", 8);
	ADD_TEST_OK(or_true_3, "d", 8);
	ADD_TEST_OK(pass_string_argument_to_game_fn, "d", 8);
	ADD_TEST_OK(pass_string_argument_to_helper_fn, "d", 8);
	ADD_TEST_OK(remainder_negative_negative, "d", 8);
	ADD_TEST_OK(remainder_negative_positive, "d", 8);
	ADD_TEST_OK(remainder_positive_negative, "d", 8);
	ADD_TEST_OK(remainder_positive_positive, "d", 8);
	ADD_TEST_OK(resource_and_entity, "d", 8);
	ADD_TEST_OK(resource_can_contain_dot_1, "d", 8);
	ADD_TEST_OK(resource_can_contain_dot_2, "d", 8);
	ADD_TEST_OK(resource_can_contain_dot_3, "d", 8);
	ADD_TEST_OK(resource_can_contain_dot_dot_1, "d", 8);
	ADD_TEST_OK(resource_can_contain_dot_dot_2, "d", 8);
	ADD_TEST_OK(resource_can_contain_dot_dot_3, "d", 8);
	ADD_TEST_OK(resource_duplicate, "d", 8);
	ADD_TEST_OK(return, "d", 8);
	ADD_TEST_OK(return_from_on_fn, "d", 8);
	ADD_TEST_OK(return_from_on_fn_minimal, "d", 8);
	ADD_TEST_OK(return_with_no_value, "d", 8);
	ADD_TEST_OK(same_variable_name_in_different_functions, "e", 8);
	ADD_TEST_OK(spill_args_to_game_fn, "d", 8);
	ADD_TEST_OK(spill_args_to_game_fn_subless, "d", 8);
	ADD_TEST_OK(spill_args_to_helper_fn, "d", 12);
	ADD_TEST_OK(spill_args_to_helper_fn_32_bit_f32, "d", 12);
	ADD_TEST_OK(spill_args_to_helper_fn_32_bit_i32, "d", 12);
	ADD_TEST_OK(spill_args_to_helper_fn_32_bit_string, "d", 12);
	ADD_TEST_OK(spill_args_to_helper_fn_subless, "d", 12);
	ADD_TEST_OK(stack_16_byte_alignment, "d", 8);
	ADD_TEST_OK(stack_16_byte_alignment_midway, "d", 8);
	ADD_TEST_OK(string_can_be_passed_to_helper_fn, "d", 8);
	ADD_TEST_OK(string_duplicate, "d", 8);
	ADD_TEST_OK(string_eq_false, "d", 8);
	ADD_TEST_OK(string_eq_true, "d", 8);
	ADD_TEST_OK(string_eq_true_empty, "d", 8);
	ADD_TEST_OK(string_ne_false, "d", 8);
	ADD_TEST_OK(string_ne_false_empty, "d", 8);
	ADD_TEST_OK(string_ne_true, "d", 8);
	ADD_TEST_OK(sub_rsp_32_bits_local_variables_i32, "d", 8);
	ADD_TEST_OK(sub_rsp_32_bits_local_variables_id, "d", 8);
	ADD_TEST_OK(subtraction_negative_result, "d", 8);
	ADD_TEST_OK(subtraction_positive_result, "d", 8);
	ADD_TEST_OK(variable, "d", 8);
	ADD_TEST_OK(variable_does_not_shadow_in_different_if_statement, "d", 8);
	ADD_TEST_OK(variable_reassignment, "d", 8);
	ADD_TEST_OK(variable_reassignment_does_not_dealloc_outer_variable, "d", 8);
	ADD_TEST_OK(variable_string_global, "d", 16);
	ADD_TEST_OK(variable_string_local, "d", 8);
	ADD_TEST_OK(void_function_early_return, "d", 8);
	ADD_TEST_OK(while_false, "d", 8);
	ADD_TEST_OK(write_to_global_variable, "d", 16);
}

int main(int argc, char *argv[]) {
	if (grug_init(runtime_error_handler, "mod_api.json", "tests", "mod_dlls", 10)) {
		fprintf(stderr, "grug_init() error: %s (detected by grug.c:%d)\n", grug_error.msg, grug_error.grug_c_line_number);
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++) {
		whitelisted_tests[whitelisted_tests_size++] = argv[i];
	}

	add_error_tests();
	add_runtime_error_tests();
	add_ok_tests();

	if (whitelisted_tests_size == 0) {
		CHECK_THAT_EVERY_TEST_DIRECTORY_HAS_A_FUNCTION(err);
		CHECK_THAT_EVERY_TEST_DIRECTORY_HAS_A_FUNCTION(err_runtime);
		CHECK_THAT_EVERY_TEST_DIRECTORY_HAS_A_FUNCTION(ok);
	}

	if (err_test_datas_size + err_runtime_test_datas_size + ok_test_datas_size == 0) {
		fprintf(stderr, "No tests to execute\n");
		exit(EXIT_FAILURE);
	}

#ifdef SHUFFLES
	// If a test failed, you can reproduce it
	// by replacing `time(NULL)` with the failing test's printed seed
	unsigned int seed = time(NULL);
	printf("The seed is %u\n", seed);
	srand(seed);

	for (size_t shuffle = 0; shuffle < SHUFFLES; shuffle++) {
	SHUFFLE(error_test_datas, err_test_datas_size, struct error_test_data);
	SHUFFLE(runtime_error_test_datas, err_runtime_test_datas_size, struct runtime_error_test_data);
	SHUFFLE(ok_test_datas, ok_test_datas_size, struct ok_test_data);
#endif

	for (size_t i = 0; i < err_test_datas_size; i++) {
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

	for (size_t i = 0; i < err_runtime_test_datas_size; i++) {
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
			fn_data.expected_globals_size_value
		);

		if (data.run) {
			runtime_error_reason = NULL;
			had_runtime_error = false;
			error_handler_calls = 0;
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

#ifdef SHUFFLES
	}
#endif

	printf("\nAll tests passed! 🎉\n");
}
