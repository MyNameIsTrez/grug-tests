#include "grug.h"

#include <assert.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct m {
	int32_t w;
	char *group;
	int32_t x;
	char *name;
	int32_t y;
	int32_t z;
};

static struct m m_definition;

void define_m(int32_t w, char *group, int32_t x, char *name, int32_t y, int32_t z) {
	m_definition = (struct m){
		.w = w,
		.group = group,
		.x = x,
		.name = name,
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
		fprintf(stderr, "dlopen: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	assert(strcmp(get(handle, "define_type"), "m") == 0);

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	grug_define_fn_t define = get(handle, "define");
	define();
	assert(m_definition.w == 42);
	assert(strcmp(m_definition.group, "military") == 0);
	assert(m_definition.x == 69);
	assert(strcmp(m_definition.name, "foo") == 0);
	assert(m_definition.y == 666);
	assert(m_definition.z == 1337);

	size_t globals_size = *(size_t *)get(handle, "globals_size");
	assert(globals_size == 0);

	void *g = malloc(globals_size);
	grug_init_globals_fn_t init_globals = get(handle, "init_globals");
	init_globals(g);
	free(g);
	#pragma GCC diagnostic pop
}
