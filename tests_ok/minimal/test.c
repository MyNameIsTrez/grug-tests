#include <assert.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*define)(void);
typedef size_t (*get_globals_struct_size)(void);
typedef void (*init_globals_struct)(void *globals_struct);

struct entity {
	uint64_t a;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
struct globals {
};
#pragma GCC diagnostic pop

static struct entity entity_definition;

void define_entity(uint64_t a) {
	entity_definition = (struct entity){
		.a = a,
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

	get_globals_struct_size get_globals_struct_size = get(handle, "get_globals_struct_size");
	size_t r = get_globals_struct_size();
	assert(r == 0);

	struct globals g;
	init_globals_struct init_globals_struct = get(handle, "init_globals_struct");
	init_globals_struct(&g);
	#pragma GCC diagnostic pop
}
