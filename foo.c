#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	int64_t rsp;

	asm("mov %%rsp, %0" : "=r" (rsp));

    printf("%ld\n", rsp);
}
