section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 0

global on_fns
on_fns:
	dq on_a

section .text

extern define_d
extern initialize

global define
define:
	call define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a:
on_a:
    push rbp
    mov rbp, rsp
    sub rsp, 0x10
    mov rbp[-0x8], rdi
    mov rbp[-0xc], esi
    mov rax, rbp[-0x8]
    push rax
    mov eax, rbp[-0xc]
    push rax
    pop rsi
    pop rdi
    call helper_fib
    push rax
    pop rdi
    call initialize wrt ..plt
    mov rsp, rbp
    pop rbp
    ret

global helper_fib:
helper_fib:
	; Function prologue
    push rbp
    mov rbp, rsp
    sub rsp, 0x10
    mov rbp[-0x8], rdi
    mov rbp[-0xc], esi

	; if n == 0
    xor eax, eax
    push rax
    mov eax, rbp[-0xc]
    pop r11
    cmp rax, r11
    mov eax, 0x0
    sete al
    test rax, rax
    je $+0xc

	; or n == 1
    mov eax, 0x1
    jmp strict $+0x26
    mov eax, 0x1
    push rax
    mov eax, rbp[-0xc]
    pop r11
    cmp rax, r11
    mov eax, 0x0
    sete al
    test rax, rax
    mov eax, 0x0
    setne al
    test rax, rax
    je strict $+0xe

	; return n
    mov eax, rbp[-0xc]
    mov rsp, rbp
    pop rbp
    ret

	; helper_fib(n - 2)
    mov rax, rbp[-0x8]
    push rax
    mov eax, 0x2
    push rax
    mov eax, rbp[-0xc]
    pop r11
    sub rax, r11
    push rax
    pop rsi
    pop rdi
    call helper_fib
    push rax

	; helper_fib(n - 1)
    mov rax, rbp[-0x8]
    push rax
    mov eax, 0x1
    push rax
    mov eax, rbp[-0xc]
    pop r11
    sub rax, r11
    push rax
    pop rsi
    pop rdi
    sub rsp, 0x8
    call helper_fib
    add rsp, 0x8
    pop r11

	; helper_fib(n - 1) + helper_fib(n - 2)
    add rax, r11

	; return helper_fib(n - 1) + helper_fib(n - 2)
    mov rsp, rbp
    pop rbp
    ret

	; Function epilogue
    mov rsp, rbp
    pop rbp
    ret
