#include "grug.h"

#include "mod_api.h"

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

void game_fn_nothing(void) {}
int32_t game_fn_magic(void) { return 0; }
void game_fn_initialize(int32_t x) {}
void game_fn_initialize_bool(bool b) {}
int32_t game_fn_identity(int32_t x) { return 0; }
int32_t game_fn_max(int32_t x, int32_t y) { return 0; }
void game_fn_say(char *message) {}
float game_fn_sin(float x) { return 0; }
float game_fn_cos(float x) { return 0; }
void game_fn_mega(float f1, int32_t i1, bool b1, float f2, float f3, float f4, bool b2, int32_t i2, float f5, float f6, float f7, float f8, int32_t i3, char *str) {}
bool game_fn_is_friday(void) { return 0; }
void game_fn_set_is_happy(bool is_happy) {}
void game_fn_mega_f32(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9) {}
void game_fn_mega_i32(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7) {}

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

		initialized = true;
	}

	char* grug_path = buf_to_file(data, size);
	if (grug_path == NULL) {
		exit(EXIT_FAILURE);
	}

	bool had_runtime_error = false;

	// If there wasn't an error generating the dll
	if (!grug_test_regenerate_dll(grug_path, dll_path)) {
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
		init_globals(g);

		struct d_on_fns *on_fns = dlsym(handle, "on_fns");
		if (on_fns && on_fns->a) {
			if (grug_mod_had_runtime_error()) {
				had_runtime_error = true;
			}

			if (!had_runtime_error) {
				on_fns->a(g);
			}
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
