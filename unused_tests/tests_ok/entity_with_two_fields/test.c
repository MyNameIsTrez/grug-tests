#include <assert.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef size_t (*get_globals_size)(void);
typedef void (*init_globals)(void *globals_struct);

struct entity {
	uint64_t a;
	uint64_t b;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
struct globals {
};
#pragma GCC diagnostic pop

void *get(void *handle, char *label) {
	void *p = dlsym(handle, label);
	if (!p) {
		fprintf(stderr, "dlsym: %s", dlerror());
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

	assert(strcmp(get(handle, "define_type"), "entity") == 0);

	struct entity e = *(struct entity *)get(handle, "define");
	assert(e.a == 42);
	assert(e.b == 69);

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	get_globals_size f = get(handle, "get_globals_size");
	size_t r = f();
	assert(r == 0);

	struct globals g;
	((init_globals)get(handle, "init_globals"))(&g);
	#pragma GCC diagnostic pop
}
