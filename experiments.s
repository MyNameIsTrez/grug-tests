; clear && nasm experiments.s -f elf64 -O0 && objdump -D experiments.o -M intel && gcc experiments.o && ./a.out

global main
main:
	; mov rax, 42
	mov eax, 0x3f99999a
	movd xmm0, eax
	ret
