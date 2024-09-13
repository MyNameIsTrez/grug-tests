#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

struct magic_on_fns {
    int (*magic)(void);
};

int main(void) {
    void *l = dlopen("./magic.dylib", RTLD_NOW);
    if (l == NULL) {
        printf("dlopen failed: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    void *on_fns = dlsym(l, "on_fns");
    if (on_fns == NULL) {
        printf("dlsym failed: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    printf("magic: %d\n", ((struct magic_on_fns *)on_fns)->magic());
}
