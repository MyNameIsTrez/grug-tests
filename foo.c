#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	// int64_t rsp;

	fprintf(stderr, "starting by sleeping for 1 second\n");
	sleep(1);

#ifdef __x86_64__
	fprintf(stderr, "__x86_64__\n");
#elif __i386__
	fprintf(stderr, "__i386__\n");
#elif __arm__
	fprintf(stderr, "__arm__\n");
#else
	fprintf(stderr, "none!\n");
#endif

	fprintf(stderr, "stopping by sleeping for 1 second\n");
	sleep(1);

	// #pragma GCC diagnostic push
	// #pragma GCC diagnostic ignored "-Wlanguage-extension-token"

	// asm("mov %%rsp, %0" : "=r" (rsp));
	// asm("mov %%sp, %0" : "=r" (rsp));

	// #pragma GCC diagnostic pop

    // fprintf(stderr, "%lld\n", rsp);
}
