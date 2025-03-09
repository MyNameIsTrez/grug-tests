section .data

align 8
global globals_size
globals_size: dq 8

resource_bar:
	db "tests/ok/resource_twice/bar.txt", 0
resource_foo:
	db "tests/ok/resource_twice/foo.txt", 0

align 8
global resources_size
resources_size: dq 2

global resources
resources:
	dq resource_bar
	dq resource_foo

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
