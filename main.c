#include <assert.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct magic_on_fns {
//     int (*magic)(void);
// };

static bool streq(char *a, char *b) {
	return strcmp(a, b) == 0;
}

static void handle_dlerror(char *function_name) {
	char *err = dlerror();
	if (!err) {
		printf("dlerror() was asked to find an error string for %s(), but it couldn't find one", function_name);
		exit(EXIT_FAILURE);
	}

	printf("%s: %s\n", function_name, err);
	exit(EXIT_FAILURE);
}

static void *get(void *dll, char *label) {
	void *p = dlsym(dll, label);
	if (!p) {
		printf("dlsym: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}
	return p;
}

int main(void) {
	printf("open dll\n");
    void *dll = dlopen("./magic.dylib", RTLD_NOW);
	if (!dll) {
		handle_dlerror("dlopen");
	}

	printf("get define_type\n");
	assert(streq(get(dll, "define_type"), "a"));

	printf("get define\n");
	void (*define)(void) = get(dll, "define");
	define();

	printf("get globals_size\n");
    size_t *globals_size_ptr = get(dll, "globals_size");
	assert(globals_size_ptr);
	printf("dereference globals_size\n");
    size_t globals_size = *globals_size_ptr;
	assert(globals_size == 0);

	printf("malloc globals_size\n");
	void *g = malloc(globals_size);
	printf("get init_globals\n");
    void (*init_globals)(void *globals) = get(dll, "init_globals");
    init_globals(g);

	// printf("get on_fns\n");
    // void *on_fns = dlsym(dll, "on_fns");

	printf("get resources_size\n");
	size_t *resources_size_ptr = get(dll, "resources_size");

	printf("get resources\n");
	char **resources = dlsym(dll, "resources");

	// printf("run magic()\n");
    // assert(((struct magic_on_fns *)on_fns)->magic() == 42);

	printf("free globals\n");
    free(g);

	printf("close dll\n");
    if (dlclose(dll)) {
        handle_dlerror("dlclose");
    }
}
