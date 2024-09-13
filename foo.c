#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	// int64_t rsp;

#ifdef defined( __x86_64__ )
	printf("__x86_64__\n");
#elif defined( __i386__ )
	printf("__i386__\n");
#elif defined( __arm__ )
	printf("__arm__\n");
#else
	printf("none!\n"); // TODO: WHY DOES THIS END UP GETTING PRINTED??
#endif

	// #pragma GCC diagnostic push
	// #pragma GCC diagnostic ignored "-Wlanguage-extension-token"

	// asm("mov %%rsp, %0" : "=r" (rsp));
	// asm("mov %%sp, %0" : "=r" (rsp));

	// #pragma GCC diagnostic pop

    // printf("%lld\n", rsp);
}
