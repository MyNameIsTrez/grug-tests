#include "grug.h"

#include <dlfcn.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// Forward declaration, since grug.h doesn't declare it
bool grug_test_regenerate_dll(char *grug_file_path, char *dll_path, char *mod);

void game_fn_define_a(void) {}
void game_fn_define_b(int32_t x) {}
void game_fn_define_c(int32_t x, int32_t y) {}
void game_fn_define_d(void) {}
void game_fn_define_e(void) {}
void game_fn_define_f(void) {}
void game_fn_define_g(void) {}
void game_fn_define_h(int32_t x) {}
void game_fn_define_i(int32_t x, int32_t y) {}
void game_fn_define_j(void) {}
void game_fn_define_k(int32_t age, char *name) {}
void game_fn_define_l(char *group, char *name) {}
void game_fn_define_m(int32_t w, char *group, bool b1, char *name, bool b2, int32_t z) {}
void game_fn_define_n(int32_t u, int32_t v, int32_t w, int32_t x, int32_t y, int32_t z) {}
void game_fn_define_o(char *u, char *v, char *w, char *x, char *y, char *z) {}
void game_fn_define_p(char *x) {}
void game_fn_define_q(char *a, char *b, char *c) {}
void game_fn_define_r(void) {}
void game_fn_define_s(void) {}
void game_fn_define_t(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8) {}
void game_fn_define_u(char *sprite_path) {}
void game_fn_define_v(char *foo, char *bar) {}
void game_fn_define_w(char *sprite_path) {}
void game_fn_define_x(char *projectile) {}
void game_fn_define_y(char *foo, char *bar) {}
void game_fn_define_z(char *projectile) {}
void game_fn_define_a2(char *sprite_path, char *projectile) {}
void game_fn_define_b2(char *sprite_path, char *projectile) {}
void game_fn_define_c2(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, int32_t i8, float f10) {}
void game_fn_define_d2(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, int32_t i8, float f9) {}
void game_fn_define_e2(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, int32_t i8, int32_t i9, int32_t i10, int32_t i11, int32_t i12, int32_t i13, int32_t i14, int32_t i15, int32_t i16, int32_t i17, int32_t i18, int32_t i19, int32_t i20, int32_t i21, int32_t i22) {}
void game_fn_define_f2(bool b) {}

void game_fn_nothing(void) {}
int32_t game_fn_magic(void) { return 0; }
void game_fn_initialize(int32_t x) {}
void game_fn_initialize_bool(bool b) {}
int32_t game_fn_identity(int32_t x) { return 0; }
int32_t game_fn_max(int32_t x, int32_t y) { return 0; }
void game_fn_say(char *message) {}
float game_fn_sin(float x) { return 0.0f; }
float game_fn_cos(float x) { return 0.0f; }
void game_fn_mega(float f1, int32_t i1, bool b1, float f2, float f3, float f4, bool b2, int32_t i2, float f5, float f6, float f7, float f8, uint64_t id, char *str) {}
bool game_fn_is_friday(void) {  return false; }
void game_fn_set_is_happy(bool is_happy) {}
void game_fn_mega_f32(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9) {}
void game_fn_mega_i32(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7) {}
void game_fn_draw(char *sprite_path) {}
void game_fn_blocked_alrm(void) {}
void game_fn_nothing_aligned(void) {}
int32_t game_fn_magic_aligned(void) { return 0; }
void game_fn_initialize_aligned(int32_t x) {}
void game_fn_spawn(char *name) {}
bool game_fn_has_resource(char *path) { return false; }
bool game_fn_has_entity(char *name) { return false; }
bool game_fn_has_string(char *str) { return false; }
uint64_t game_fn_get_opponent(void) { return 0; }
void game_fn_set_target(uint64_t target) {}
void game_fn_motherload(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, uint64_t id, float f9) {}
void game_fn_motherload_subless(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, uint64_t id, float f10) {}
void game_fn_offset_32_bit_f32(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7, char *s8, char *s9, char *s10, char *s11, char *s12, char *s13, char *s14, char *s15, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, int32_t g) {}
void game_fn_offset_32_bit_i32(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t g) {}
void game_fn_offset_32_bit_string(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, char *s1, char *s2, char *s3, char *s4, char *s5, int32_t g) {}
void game_fn_talk(char *message1, char *message2, char *message3, char *message4) {}

// Source: https://github.com/google/security-research-pocs/blob/d10780c3ddb8070dff6c5e5862c93c01392d1727/autofuzz/fuzz_utils.cc#L10
int ignore_stdout(void) {
	int fd = open("/dev/null", O_WRONLY);
	if (fd == -1) {
		warn("open(\"/dev/null\") failed");
		return -1;
	}

	int ret = 0;
	if (dup2(fd, STDOUT_FILENO) == -1) {
		warn("failed to redirect stdout to /dev/null\n");
		ret = -1;
	}

	if (close(fd) == -1) {
		warn("close");
		ret = -1;
	}

	return ret;
}

// Source: https://github.com/google/security-research-pocs/blob/d10780c3ddb8070dff6c5e5862c93c01392d1727/autofuzz/fuzz_utils.cc#L31
int delete_file(const char *pathname) {
	int ret = unlink(pathname);
	if (ret == -1) {
		warn("failed to delete \"%s\"", pathname);
	}

	free((void *)pathname);

	return ret;
}

// Source: https://github.com/google/security-research-pocs/blob/d10780c3ddb8070dff6c5e5862c93c01392d1727/autofuzz/fuzz_utils.cc#L42
char *buf_to_file(const uint8_t *buf, size_t size) {
	char *pathname = strdup("/dev/shm/fuzz-XXXXXX");
	if (pathname == NULL) {
		return NULL;
	}

	int fd = mkstemp(pathname);
	if (fd == -1) {
		warn("mkstemp(\"%s\")", pathname);
		free(pathname);
		return NULL;
	}

	size_t pos = 0;
	while (pos < size) {
		int nbytes = write(fd, &buf[pos], size - pos);
		if (nbytes <= 0) {
		if (nbytes == -1 && errno == EINTR) {
			continue;
		}
		warn("write");
		goto err;
		}
		pos += nbytes;
	}

	if (close(fd) == -1) {
		warn("close");
		goto err;
	}

	return pathname;

err:
	delete_file(pathname);
	return NULL;
}

static void *get(void *handle, char *label) {
	void *p = dlsym(handle, label);
	if (!p) {
		fprintf(stderr, "dlsym: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}
	return p;
}

static void runtime_error_handler(char *reason, enum grug_runtime_error_type type, char *on_fn_name, char *on_fn_path) {}

// Source: https://github.com/google/security-research-pocs/blob/649b6ed74c842f533d15410f13d94aada96375ef/autofuzz/alembic_fuzzer.cc#L293
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
	static bool initialized = false;
	static char dll_path[] = "./fuzz.dll";

	if (!initialized) {
    	ignore_stdout();

		int fd = open(dll_path, O_CREAT | O_TRUNC | O_RDWR, 0644);
		if (fd == -1) {
			perror("open");
			return EXIT_FAILURE;
		}

		grug_init(runtime_error_handler, "mod_api.json", "fuzzing");

		initialized = true;
	}

	char* grug_path = buf_to_file(data, size);
	if (grug_path == NULL) {
		exit(EXIT_FAILURE);
	}

	// If there wasn't an error generating the dll
	if (!grug_test_regenerate_dll(grug_path, dll_path, "fuzzing")) {
		void *handle = dlopen(dll_path, RTLD_NOW);
		if (!handle) {
			fprintf(stderr, "dlopen: %s\n", dlerror());
			exit(EXIT_FAILURE);
		}

		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wpedantic"

		grug_define_fn_t define = get(handle, "define");
		define();

		size_t globals_size = *(size_t *)get(handle, "globals_size");

		void *g = malloc(globals_size);
		grug_init_globals_fn_t init_globals = get(handle, "init_globals");
		init_globals(g, 42);

		struct d_on_fns *on_fns = dlsym(handle, "on_fns");
		if (on_fns && on_fns->a) {
			on_fns->a(g);
		}

		free(g);

		#pragma GCC diagnostic pop

		if (dlclose(handle)) {
			fprintf(stderr, "dlclose: %s\n", dlerror());
			exit(EXIT_FAILURE);
		}
	}

	if (delete_file(grug_path) != 0) {
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
