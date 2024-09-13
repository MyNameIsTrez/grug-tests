typedef long long int64_t;

int printf(const char *restrict format, ...);

int main(void) {
	int64_t rsp;

	asm("mov sp, %0\n\t" : "=r" (rsp));
	// asm("mov %0, sp\n\t" : "=r" (rsp));

    printf("%lld\n", rsp);
}
