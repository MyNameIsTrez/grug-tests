section .data

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

%include "tests/utils/defines.s"
%include "tests/utils/macros.s"

extern grug_runtime_error_handler
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern setjmp
extern grug_get_runtime_error_reason
extern game_fn_initialize_bool

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret
