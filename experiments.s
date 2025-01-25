; clear && nasm experiments.s -f elf64 -O0 && objdump -d experiments.o -M intel && clang experiments.o -z noexecstack && ./a.out

extern grug_max_rsp
extern grug_runtime_error_jmp_buffer
extern longjmp

%define GRUG_ON_FN_STACK_OVERFLOW 1

global main
main:
	mov rax, [rel grug_max_rsp wrt ..got]
	cmp rsp, [rax]
	jg $+0x17
	mov esi, 1 + GRUG_ON_FN_STACK_OVERFLOW
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call longjmp wrt ..plt
	ret
