#include "grug.h"

#include <assert.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void on_a(void);
void on_b(void);

struct i {
	int32_t x;
	int32_t y;
};

struct my_on_fns {
	typeof(on_a) *a;
	typeof(on_b) *b;
};

static struct i i_definition;

void define_i(int32_t x, int32_t y) {
	i_definition = (struct i){
		.x = x,
		.y = y,
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

	assert(strcmp(get(handle, "define_type"), "i") == 0);

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	grug_define_fn_t define = get(handle, "define");
	define();
	assert(i_definition.x == 42);
	assert(i_definition.y == 69);

	grug_get_globals_size_fn_t get_globals_size_fn = get(handle, "get_globals_size");
	size_t globals_size = get_globals_size_fn();
	assert(globals_size == 0);

	void *g = malloc(globals_size);
	grug_init_globals_fn_t init_globals = get(handle, "init_globals");
	init_globals(g);
	free(g);

	struct my_on_fns *on_fns = get(handle, "on_fns");
	assert(on_fns->a == NULL);
	on_fns->b();
	#pragma GCC diagnostic pop
}
