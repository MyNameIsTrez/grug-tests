; clear && nasm experiments.s -f elf64 -O0 && objdump -D experiments.o -M intel

global foo
foo:
	mov rax, 42
	ret
