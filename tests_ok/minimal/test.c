#include <assert.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*define)(void);
typedef size_t (*get_globals_size)(void);
typedef void (*init_globals)(void *globals);

struct entity {
	uint64_t a;
	uint64_t b;
};

static struct entity entity_definition;

void define_entity(uint64_t a, uint64_t b) {
	entity_definition = (struct entity){
		.a = a,
		.b = b,
	};
}

static void *get(void *handle, char *label) {
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

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	define define = get(handle, "define");
	define();
	assert(entity_definition.a == 42);
	assert(entity_definition.b == 69);

	get_globals_size get_globals_size = get(handle, "get_globals_size");
	size_t globals_size = get_globals_size();
	assert(globals_size == 8);

	void *g = malloc(globals_size);
	init_globals init_globals = get(handle, "init_globals");
	init_globals(g);
	assert(((uint32_t*)g)[0] == 420);
	assert(((uint32_t*)g)[1] == 1337);
	free(g);
	#pragma GCC diagnostic pop
}
