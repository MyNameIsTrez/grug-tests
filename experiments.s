; clear && nasm experiments.s -f elf64 -O0 && objdump -d experiments.o -M intel && clang experiments.o -z noexecstack && ./a.out

global main
main:
	ret
