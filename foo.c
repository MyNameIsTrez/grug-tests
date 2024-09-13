#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	// int64_t rsp;

#ifdef __x86_64__
	printf("__x86_64__\n");
#elif _M_X64
	printf("_M_X64\n");
#elif __i386__
	printf("__i386__\n");
#elif _M_IX86
	printf("_M_IX86\n");
#elif __aarch64__
	printf("__aarch64__\n");
#elif _M_ARM64
	printf("_M_ARM64\n");
#elif __arm__
	printf("__arm__\n");
#elif _M_ARM
	printf("_M_ARM\n");
#elif __EMSCRIPTEN__
	printf("__EMSCRIPTEN__\n");
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
