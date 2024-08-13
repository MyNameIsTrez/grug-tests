#include "grug.h"

#include "mod_api.h"

#include <assert.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLOBAL_LEN sizeof("globalXXXX")

void game_fn_define_a(void) {
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

	assert(strcmp(get(handle, "define_type"), "a") == 0);

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	grug_define_fn_t define = get(handle, "define");
	define();

	size_t globals_size = *(size_t *)get(handle, "globals_size");
	assert(globals_size == GLOBAL_LEN * 1000);

	void *g = malloc(globals_size);
	grug_init_globals_fn_t init_globals = get(handle, "init_globals");
	init_globals(g);

	for (int32_t i = 0; i < 1000; i++) {
		static char global_name[GLOBAL_LEN];
		snprintf(global_name, GLOBAL_LEN, "global%04d", i + 1);
		// fprintf(stderr, "%s\n", ((char**)g)[i]);
		assert(strcmp(((char**)g)[i], global_name) == 0);
	}

	free(g);
	#pragma GCC diagnostic pop
}
