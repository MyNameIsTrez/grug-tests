#include "grug.h"

#include <assert.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct n {
	int32_t u;
	int32_t v;
	int32_t w;
	int32_t x;
	int32_t y;
	int32_t z;
};

static struct n n_definition;

void define_n(int32_t u, int32_t v, int32_t w, int32_t x, int32_t y, int32_t z) {
	n_definition = (struct n){
		.u = u,
		.v = v,
		.w = w,
		.x = x,
		.y = y,
		.z = z,
	};
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
		fprintf(stderr, "dlopen: %s", dlerror());
		exit(EXIT_FAILURE);
	}

	assert(strcmp(get(handle, "define_type"), "n") == 0);

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	grug_define_fn_t define = get(handle, "define");
	define();
	assert(n_definition.u == 1);
	assert(n_definition.v == 2);
	assert(n_definition.w == 3);
	assert(n_definition.x == 4);
	assert(n_definition.y == 5);
	assert(n_definition.z == 6);

	size_t globals_size = *(size_t *)get(handle, "globals_size");
	assert(globals_size == 0);

	void *g = malloc(globals_size);
	grug_init_globals_fn_t init_globals = get(handle, "init_globals");
	init_globals(g);
	free(g);
	#pragma GCC diagnostic pop
}