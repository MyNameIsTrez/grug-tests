section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 4

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
	mov dword rdi[byte 0x0], 42
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, 0x8
    mov rbp[-0x8], rdi

	; foo + 1
	mov eax, 1
	push rax
	mov rax, rbp[-0x8]
	mov eax, [rax+0x0]
	pop r11
	add rax, r11

	; foo = foo + 1
	mov r11, rbp[-0x8]
	mov [r11+0x0], eax

	; initialize(foo)
	mov rax, rbp[-0x8]
	mov eax, [rax+0x0]
	push rax

	pop rdi
	call initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
