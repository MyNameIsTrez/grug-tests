section .data

global define_type
define_type: db "t", 0

align 8
global globals_size
globals_size: dq 8

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern game_fn_define_t

global define
define:
	sub rsp, byte 0x8
	mov eax, __?float32?__(8.0)
	movd xmm7, eax
	mov eax, __?float32?__(7.0)
	movd xmm6, eax
	mov eax, __?float32?__(6.0)
	movd xmm5, eax
	mov eax, __?float32?__(5.0)
	movd xmm4, eax
	mov eax, __?float32?__(4.0)
	movd xmm3, eax
	mov eax, __?float32?__(3.0)
	movd xmm2, eax
	mov eax, __?float32?__(2.0)
	movd xmm1, eax
	mov eax, __?float32?__(1.0)
	movd xmm0, eax
	call game_fn_define_t wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
