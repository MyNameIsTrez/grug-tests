section .data

align 8
global globals_size
globals_size: dq 12

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
	mov eax, 2
	neg eax
	mov rdi[0x8], eax
	ret
