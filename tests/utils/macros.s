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
	mov rax, [rel grug_max_rsp wrt ..got]
	mov [rax], rsp
	sub qword [rax], GRUG_STACK_LIMIT
%endmacro

%macro set_time_limit 0
	; clock_gettime(CLOCK_PROCESS_CPUTIME_ID, grug_max_time);
	mov rsi, [rel grug_max_time wrt ..got]
	push rsi
	mov edi, CLOCK_PROCESS_CPUTIME_ID
	call clock_gettime wrt ..plt
	pop rax

	; grug_max_time.sec += GRUG_ON_FN_TIME_LIMIT_SEC;
	add qword [byte rax + TV_SEC_OFFSET], GRUG_ON_FN_TIME_LIMIT_SEC

	; grug_max_time.nsec += GRUG_ON_FN_TIME_LIMIT_NS;
	add qword [byte rax + TV_NSEC_OFFSET], GRUG_ON_FN_TIME_LIMIT_NS

	; if (grug_max_time.nsec < NS_PER_SEC) goto skip;
	cmp qword [byte rax + TV_NSEC_OFFSET], NS_PER_SEC
	jl %%skip

	; grug_max_time.nsec -= NS_PER_SEC;
	sub qword [byte rax + TV_NSEC_OFFSET], NS_PER_SEC

	; grug_max_time.sec++;
	inc qword [byte rax + TV_SEC_OFFSET]
%%skip:
%endmacro

%macro check_stack_overflow 0
	mov rax, [rel grug_max_rsp wrt ..got]
	cmp rsp, [rax]
	jg %%skip
	runtime_error GRUG_ON_FN_STACK_OVERFLOW
%%skip:
%endmacro

%macro check_time_limit_exceeded 0
	; clock_gettime(CLOCK_PROCESS_CPUTIME_ID, grug_current_time);
	mov rsi, [rel grug_current_time wrt ..got]
	push rsi
	mov edi, CLOCK_PROCESS_CPUTIME_ID
	call clock_gettime wrt ..plt
	pop rax
	mov r11, [rel grug_max_time wrt ..got]

	; if (grug_current_time.sec < grug_max_time.sec) goto skip;
	mov r10, [byte r11 + TV_SEC_OFFSET]
	cmp [byte rax + TV_SEC_OFFSET], r10
	jl %%skip

	; if (grug_current_time.sec > grug_max_time.sec) goto longjmp;
	jg %%exceeded

	; if (grug_current_time.nsec > grug_max_time.nsec) goto longjmp;
	mov r10, [byte r11 + TV_NSEC_OFFSET]
	cmp [byte rax + TV_NSEC_OFFSET], r10
	jg %%exceeded

	; goto skip;
	jmp short %%skip
%%exceeded:
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
