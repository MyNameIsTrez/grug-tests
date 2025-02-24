section .data

global define_type
define_type: db "v", 0

align 8
global globals_size
globals_size: dq 8

resource:
	db "tests/ok/resource_duplicate/foo.txt", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq resource

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern game_fn_define_v

global define
define:
	sub rsp, byte 0x8
	lea rax, [rel resource]
	mov rsi, rax
	lea rax, [rel resource]
	mov rdi, rax
	call game_fn_define_v wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
