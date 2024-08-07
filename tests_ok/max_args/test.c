#include "grug.h"

#include "mod_api.h"

#include <assert.h>
#include <dlfcn.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void game_fn_define_d(void) {
}

static bool fn_mega_was_called = false;
void game_fn_mega(float f1, int32_t i1, bool b1, float f2, float f3, float f4, bool b2, int32_t i2, float f5, float f6, float f7, float f8, int32_t i3, char *str) {
	fn_mega_was_called = true;

	assert(f1 == 1.0f);
	assert(i1 == 21);
	assert(b1 == true);
	assert(f2 == 2.0f);
	assert(f3 == 3.0f);
	assert(f4 == 4.0f);
	assert(b2 == false);
	assert(i2 == 1337);
	assert(f5 == 5.0f);
	assert(f6 == 6.0f);
	assert(f7 == 7.0f);
	assert(f8 == 8.0f);
	assert(i3 == 8192);
	assert(strcmp(str, "foo") == 0);
}

static void *get(void *handle, char *label) {
	void *p = dlsym(handle, label);
	if (!p) {
		fprintf(stderr, "dlsym: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}
	return p;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <so_path>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *so_path = argv[1];

	void *handle = dlopen(so_path, RTLD_NOW);
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
	assert(!fn_mega_was_called);
	on_fns->a(g);
	assert(fn_mega_was_called);

	free(g);
	#pragma GCC diagnostic pop
}
