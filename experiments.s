; clear && nasm experiments.s -f elf64 -O0 && objdump -d experiments.o -M intel && clang experiments.o -z noexecstack && ./a.out

extern grug_max_time

%define GRUG_ON_FN_TIME_LIMIT_EXCEEDED 2
%define CLOCK_PROCESS_CPUTIME_ID 2
%define TV_SEC_OFFSET 0
%define TV_NSEC_OFFSET 8
%define GRUG_ON_FN_TIME_LIMIT_MS 10
%define NS_PER_MS 1000000
%define NS_PER_SEC 1000000000

global main
main:
	jmp short $+0x16
	; jmp foo
	; nop
	; nop
	; foo:
	; nop
	ret
