; clear && nasm experiments.s -f elf64 -O0 && objdump -D experiments.o -M intel && gcc experiments.o && ./a.out

global main
main:
	movss rbp[-0x4], xmm0
	movss rbp[-0x4], xmm1
	movss rbp[-0x4], xmm2
	movss rbp[-0x4], xmm3
	movss rbp[-0x4], xmm4
	movss rbp[-0x4], xmm5
	movss rbp[-0x4], xmm6
	movss rbp[-0x4], xmm7

	ret
