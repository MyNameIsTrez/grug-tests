section .data

align 8
global globals_size
globals_size: dq 8

entity_type:
	db 0
entity:
	db "wow:foo", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 1

global entities
entities:
	dq entity

global entity_types
entity_types:
	dq entity_type

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
