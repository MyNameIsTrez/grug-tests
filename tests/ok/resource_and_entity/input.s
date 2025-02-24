section .data

global define_type
define_type: db "b2", 0

align 8
global globals_size
globals_size: dq 8

entity_type:
	db 0
entity:
	db "ok:foo", 0
resource:
	db "tests/ok/resource_and_entity/foo.txt", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq resource

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

extern game_fn_define_b2

global define
define:
	sub rsp, byte 0x8
	lea rax, [rel entity]
	mov rsi, rax
	lea rax, [rel resource]
	mov rdi, rax
	call game_fn_define_b2 wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
