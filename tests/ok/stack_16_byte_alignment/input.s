section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/ok/stack_16_byte_alignment/input-d.grug", 0
on_fn_name:
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern grug_runtime_error_handler
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_nothing_aligned
extern game_fn_initialize_aligned

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10 ; Change 0x10 to 0x18 to see the unaligned access crash
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_on_fn_name_and_path

	call game_fn_nothing_aligned wrt ..plt

	; add rsp, 0x8 ; Uncomment to see the unaligned access crash

	; This shows that no matter how many arguments there are,
	; we just need to make sure to have decremented rsp by multiples of 16
	mov eax, 42
	push rax
	pop rdi
	call game_fn_initialize_aligned wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

.fast:
	call game_fn_nothing_aligned wrt ..plt

	; add rsp, 0x8 ; Uncomment to see the unaligned access crash

	; This shows that no matter how many arguments there are,
	; we just need to make sure to have decremented rsp by multiples of 16
	mov eax, 42
	push rax
	pop rdi
	call game_fn_initialize_aligned wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
