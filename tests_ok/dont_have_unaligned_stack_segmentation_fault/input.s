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
extern alarm
extern game_fn_nothing
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
	sub rsp, byte 0x10 ; Change 0x10 to 0x8 to see the unaligned access crash
	mov rbp[-0x8], rdi
	mov edi, 1
	call alarm wrt ..plt

	call game_fn_nothing wrt ..plt

	; add rsp, 0x8 ; Uncomment to see the unaligned access crash

	; This shows that no matter how many arguments there are,
	; we just need to make sure to have decremented rsp by multiples of 16
	mov eax, 42
	push rax
	pop rdi
	call game_fn_initialize wrt ..plt

	xor edi, edi
	call alarm wrt ..plt
	mov rsp, rbp
	pop rbp
	ret
