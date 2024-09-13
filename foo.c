typedef long long int64_t;

int printf(const char *restrict format, ...);

int main(void) {
	int64_t rsp;

	asm("mov sp, %0" : "=r" (rsp));

    printf("%lld\n", rsp);
}
