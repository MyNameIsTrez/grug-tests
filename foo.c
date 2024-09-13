#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int64_t rsp;

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wlanguage-extension-token"

	// asm("mov %%rsp, %0" : "=r" (rsp));
	asm("movq %%rsp, %0" : "=r" (rsp));

	#pragma GCC diagnostic pop

    printf("%lld\n", rsp);
}
