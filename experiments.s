; clear && nasm experiments.s -f elf64 -O0 && objdump -d experiments.o -M intel && clang experiments.o -z noexecstack && ./a.out
; clear && nasm experiments.s -f elf64 -O0 && objdump -d experiments.o -M intel
; clear && nasm experiments.s -f elf64 -O0 && clang experiments.o -z noexecstack && ./a.out; echo $?

global main
main:
	ret
