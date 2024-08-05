; clear && nasm experiments.s -f elf64 -O0 && objdump -D experiments.o -M intel && gcc experiments.o && ./a.out

global main
main:
	movd xmm1,r11d
    addss xmm0,xmm1

	ret
