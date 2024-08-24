#include "grug.h"

#include "mod_api.h"

#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void game_fn_define_d(void) {
}

static bool fn_nothing_was_called = false;
void game_fn_nothing(void) {
	fn_nothing_was_called = true;

	// From https://stackoverflow.com/a/2114249/13279557
	int64_t rsp;
	asm ("mov %%rsp, %0" : "=r" (rsp) );

	// We need this in order to ensure that the C compiler will 16-byte align
	// this function with a function prologue, cause we assert that the rsp is divisible by 16 after the function prologue
	printf(":)\n");

	assert((rsp & 0xf) == 0);
}

static bool fn_initialize_was_called = false;
void game_fn_initialize(int32_t x) {
	fn_initialize_was_called = true;

	// From https://stackoverflow.com/a/2114249/13279557
	int64_t rsp;
	asm ("mov %%rsp, %0" : "=r" (rsp) );

	// We need this in order to ensure that the C compiler will 16-byte align
	// this function with a function prologue, cause we assert that the rsp is divisible by 16 after the function prologue
	printf(":)\n");

	assert((rsp & 0xf) == 0);

	assert(x == 42);
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
	assert(!fn_nothing_was_called);
	assert(!fn_initialize_was_called);
	on_fns->a(g);
	assert(fn_nothing_was_called);
	assert(fn_initialize_was_called);

	free(g);
	#pragma GCC diagnostic pop
}
