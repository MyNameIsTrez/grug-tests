section .data

align 8
global globals_size
globals_size: dq 8

resource:
	db "tests/ok/resource_in_define/foo.txt", 0

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

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
