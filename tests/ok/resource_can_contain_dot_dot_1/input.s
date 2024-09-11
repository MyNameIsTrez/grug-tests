section .data

global define_type
define_type: db "u", 0

align 8
global globals_size
globals_size: dq 0

global strings
strings:
	db "tests/ok/resource_can_contain_dot_dot_1/..foo", 0

align 8
global resources_size
resources_size: dq 1

global resources
resources:
	dq strings + 0

global resource_mtimes
resource_mtimes:
	dq 0

section .text

extern grug_on_fn_name
extern grug_on_fn_path
extern grug_block_mask

extern game_fn_define_u

global define
define:
	lea rax, strings[rel 0]
	mov rdi, rax
	call game_fn_define_u wrt ..plt
	ret

global init_globals
init_globals:
	ret
