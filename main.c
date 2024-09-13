#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

// struct magic_on_fns {
//     int (*magic)(void);
// };

// nasm -fmacho64 magic.s -o magic.dylib
// clang main.c && ./a.out
// xxd magic.dylib
// llvm-readelf magic.dylib
// llvm-objdump -D magic.dylib
int main(void) {
    void *l = dlopen("./magic.dylib", RTLD_NOW);
    if (l == NULL) {
        printf("dlopen failed: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    int (*magic)(void) = dlsym(l, "magic");
    if (magic == NULL) {
        printf("dlsym failed: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    printf("magic: %d\n", magic());

    // void *on_fns = dlsym(l, "on_fns");
    // if (on_fns == NULL) {
    //     printf("dlsym failed: %s\n", dlerror());
    //     return EXIT_FAILURE;
    // }

    // printf("magic: %d\n", ((struct magic_on_fns *)on_fns)->magic());
}
