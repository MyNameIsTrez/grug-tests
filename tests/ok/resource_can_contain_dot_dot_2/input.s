section .data

global define_type
define_type: db "u", 0

align 8
global globals_size
globals_size: dq 8

resource:
	db "tests/ok/resource_can_contain_dot_dot_2/foo..", 0

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

extern game_fn_define_u

global define
define:
	sub rsp, byte 0x8
	lea rax, [rel resource]
	mov rdi, rax
	call game_fn_define_u wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
