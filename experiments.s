; clear && nasm experiments.s -f elf64 -O0 && objdump -D experiments.o -M intel && gcc experiments.o && ./a.out

global main
main:
	mov edi, 42
	mov esi, 42
	mov edx, 42
	mov ecx, 42
	mov r8d, 42
	mov r9d, 42

	ret
