section .data

align 8
global globals_size
globals_size: dq 8

z:
	db "z", 0
y:
	db "y", 0
x:
	db "x", 0
w:
	db "w", 0
v:
	db "v", 0
u:
	db "u", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
