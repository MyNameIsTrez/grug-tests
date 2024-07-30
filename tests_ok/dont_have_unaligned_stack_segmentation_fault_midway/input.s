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

extern game_fn_define_d
extern game_fn_magic
extern game_fn_initialize

global define
define:
	call game_fn_define_d wrt ..plt
	ret

global init_globals
init_globals:
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, 0x10
    mov rbp[-0x8], rdi

	; magic() + 42
	mov eax, 42
	push rax
	sub rsp, 0x8 ; Comment this out along with the `add rsp, 0x8` below the call to see the unaligned access crash
	call game_fn_magic wrt ..plt
	add rsp, 0x8
	pop r11
	add rax, r11
	push rax

	; initialize(magic() + 42)
	pop rdi
	call game_fn_initialize wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
