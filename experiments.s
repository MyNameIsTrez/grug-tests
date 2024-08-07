; clear && nasm experiments.s -f elf64 -O0 && objdump -D experiments.o -M intel && gcc experiments.o && ./a.out

global main
main:
	ret
