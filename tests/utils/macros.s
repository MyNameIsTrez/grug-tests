%macro save_on_fn_name_and_path 0
	mov rax, [rel grug_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_fn_name wrt ..got]
	lea r11, [rel on_fn_name]
	mov [rax], r11
%endmacro

%macro save_on_fn_name_and_path_on_a 0
	mov rax, [rel grug_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_fn_name wrt ..got]
	lea r11, [rel on_fn_name_a]
	mov [rax], r11
%endmacro

%macro save_on_fn_name_and_path_on_b 0
	mov rax, [rel grug_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_fn_name wrt ..got]
	lea r11, [rel on_fn_name_b]
	mov [rax], r11
%endmacro

%macro save_on_fn_name_and_path_on_c 0
	mov rax, [rel grug_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_fn_name wrt ..got]
	lea r11, [rel on_fn_name_c]
	mov [rax], r11
%endmacro

%macro save_init_globals_fn_name_and_path 0
	mov rax, [rel grug_fn_path wrt ..got]
	lea r11, [rel init_globals_fn_path]
	mov [rax], r11

	mov rax, [rel grug_fn_name wrt ..got]
	lea r11, [rel init_globals_fn_name]
	mov [rax], r11
%endmacro

%macro clear_has_runtime_error_happened 0
	mov rax, [rel grug_has_runtime_error_happened wrt ..got]
	mov [rax], byte 0
%endmacro

%macro runtime_error 1
	mov rax, [rel grug_has_runtime_error_happened wrt ..got]
	mov [rax], byte 1

	mov edi, %1
	call grug_call_runtime_error_handler wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
%endmacro

%macro return_if_runtime_error 0
	mov r11, [rel grug_has_runtime_error_happened wrt ..got]
	mov r11b, [r11]
	test r11b, r11b
	je %%skip

	mov rsp, rbp
	pop rbp
	ret
%%skip:
%endmacro

%macro set_max_rsp 0
    ; Prepare TLS descriptor in rdi
	;
	; A simple `mov rax, [rel grug_max_rsp wrt ..got]` doesn't work,
	; as `grug_max_rsp` is marked `thread_local` in grug.c.
	;
	; There is unfortunately no NASM equivalent to GAS its `grug_max_rsp@TLSGD`.
	;
	; This is necessary for Minecraft, as grug sporadically and incorrectly reported a stack overflow,
	; which is caused by multiple threads calling grug on_ functions simultaneously.
    call grug_get_max_rsp_addr wrt ..plt

	; Set the max grug stack limit to the current rsp, minus GRUG_STACK_LIMIT
	mov [rax], rsp
	sub qword [rax], GRUG_STACK_LIMIT
%endmacro

%macro check_stack_overflow 0
    call grug_get_max_rsp wrt ..plt
	cmp rsp, rax
	jg %%skip
	runtime_error GRUG_ON_FN_STACK_OVERFLOW
%%skip:
%endmacro

%macro check_time_limit_exceeded 0
    call grug_is_time_limit_exceeded wrt ..plt

	test al, al
	je %%skip

	runtime_error GRUG_ON_FN_TIME_LIMIT_EXCEEDED
%%skip:
%endmacro

%macro check_division_overflow 0
	cmp eax, -2147483648
	jne %%skip
	cmp r11d, -1
	jne %%skip
	runtime_error GRUG_ON_FN_OVERFLOW
%%skip:
%endmacro

%macro check_division_by_0 0
	test r11, r11
	jne %%skip
	runtime_error GRUG_ON_FN_DIVISION_BY_ZERO
%%skip:
%endmacro

%macro check_overflow 0
	jno %%skip
	runtime_error GRUG_ON_FN_OVERFLOW
%%skip:
%endmacro

; This is deliberately not using rax, as rax holds the return value
; of the game/helper function that was just called.
%macro check_game_fn_error 0
	mov r11, [rel grug_has_runtime_error_happened wrt ..got]
	mov r11b, [r11]
	test r11b, r11b
	je %%skip

	mov edi, GRUG_ON_FN_GAME_FN_ERROR
	call grug_call_runtime_error_handler wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
%%skip:
%endmacro
