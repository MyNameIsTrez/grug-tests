section .data

global define_type
define_type: db "y", 0

align 8
global globals_size
globals_size: dq 8

entity_type:
	db 0
bar:
	db "ok:bar", 0
foo:
	db "ok:foo", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 2

global entities
entities:
	dq bar
	dq foo

global entity_types
entity_types:
	dq entity_type
	dq entity_type

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern game_fn_define_y

global define
define:
	sub rsp, byte 0x8
	lea rax, [rel bar]
	mov rsi, rax
	lea rax, [rel foo]
	mov rdi, rax
	call game_fn_define_y wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
