; clear && nasm experiments.s -f elf64 -O0 && objdump -D experiments.o

global foo
foo:
	mov rax, 42
	ret
