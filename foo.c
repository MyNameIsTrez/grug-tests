#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	// int64_t rsp;

#ifdef __x86_64__
	printf("__x86_64__\n");
#elif __i386__
	printf("__i386__\n");
#elif __arm__
	printf("__arm__\n");
#else
	printf("none!\n");
#endif

	// #pragma GCC diagnostic push
	// #pragma GCC diagnostic ignored "-Wlanguage-extension-token"

	// asm("mov %%rsp, %0" : "=r" (rsp));
	// asm("mov %%sp, %0" : "=r" (rsp));

	// #pragma GCC diagnostic pop

    // printf("%lld\n", rsp);
}
