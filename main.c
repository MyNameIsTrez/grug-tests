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
    void *dll = dlopen("./magic.dylib", RTLD_NOW);
	if (!dll) {
		handle_dlerror("dlopen");
	}

	assert(streq(get(dll, "define_type"), "a"));

	void (*define)(void) = get(dll, "define");
	define();

    size_t globals_size = *(size_t *)get(dll, "globals_size");
	assert(globals_size == 0);

	void *g = malloc(globals_size);
    void (*init_globals)(void *globals) = get(dll, "globals_size");
    init_globals(g);

    // void *on_fns = dlsym(dll, "on_fns");

	size_t *resources_size_ptr = get(dll, "resources_size");

	char **resources = dlsym(dll, "resources");

    // assert(((struct magic_on_fns *)on_fns)->magic() == 42);

    free(g);

    if (dlclose(dll)) {
        handle_dlerror("dlclose");
    }
}
