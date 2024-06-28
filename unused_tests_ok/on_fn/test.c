#include <assert.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*define)(void);
typedef size_t (*get_globals_size)(void);
typedef void (*init_globals)(void *globals);

void on_a(void);

struct a {
	int32_t a;
};

struct my_on_fns {
	typeof(on_a) *a;
};

static struct a a_definition;

void define_a(int32_t a) {
	a_definition = (struct a){
		.a = a,
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

	assert(strcmp(get(handle, "define_type"), "a") == 0);

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	define define = get(handle, "define");
	define();
	assert(a_definition.a == 42);

	get_globals_size get_globals_size = get(handle, "get_globals_size");
	size_t globals_size = get_globals_size();
	assert(globals_size == 0);

	void *g = malloc(globals_size);
	init_globals init_globals = get(handle, "init_globals");
	init_globals(g);
	free(g);

	struct my_on_fns *on_fns = get(handle, "on_fns");
	on_fns->a();
	#pragma GCC diagnostic pop
}
