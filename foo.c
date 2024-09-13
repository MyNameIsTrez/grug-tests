#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	// int64_t rsp;

#ifdef __x86_64__
	fprintf(stderr, "__x86_64__\n");
#elif __i386__
	fprintf(stderr, "__i386__\n");
#elif __arm__
	fprintf(stderr, "__arm__\n");
#endif

	// #pragma GCC diagnostic push
	// #pragma GCC diagnostic ignored "-Wlanguage-extension-token"

	// // asm("mov %%rsp, %0" : "=r" (rsp));
	// asm("movq %%rsp, %0" : "=r" (rsp));

	// #pragma GCC diagnostic pop

    // fprintf(stderr, "%lld\n", rsp);
}
