global define

global define_type

global get_globals_struct_size
global init_globals_struct

section .data

define:
	db 42

define_type: db "entity", 0

section .text

get_globals_struct_size:
	mov rax, 0
	ret

; TODO: Translate this:
; void init_globals_struct(void *globals_struct) {
;     memcpy(globals_struct, &(struct globals){
;     }, sizeof(struct globals));
; }
init_globals_struct:
	ret
