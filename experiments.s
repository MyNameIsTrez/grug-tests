; clear && nasm experiments.s -f elf64 -O0 && objdump -D experiments.o -M intel && gcc experiments.o && ./a.out

global main
main:
	; mov rax, 42

	mov eax, 0x3f99999a

	movd xmm0, eax
	movd xmm1, eax
	movd xmm2, eax
	movd xmm3, eax
	movd xmm4, eax
	movd xmm5, eax
	movd xmm6, eax
	movd xmm7, eax

	movd xmm8, eax
	movd xmm9, eax
	movd xmm10, eax
	movd xmm11, eax
	movd xmm12, eax
	movd xmm13, eax
	movd xmm14, eax
	movd xmm15, eax

	pop rax

	movd eax, xmm0

	ret
