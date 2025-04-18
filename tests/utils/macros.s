%macro save_init_globals_fn_name_and_path 0
	mov rax, [rel grug_fn_path wrt ..got]
	lea r11, [rel init_globals_fn_path]
	mov [rax], r11

	mov rax, [rel grug_fn_name wrt ..got]
	lea r11, [rel init_globals_fn_name]
	mov [rax], r11
%endmacro

%macro save_on_fn_name_and_path 0
	mov rax, [rel grug_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_fn_name wrt ..got]
	lea r11, [rel on_fn_name]
	mov [rax], r11
%endmacro

%macro init_globals_fn_error_handling 0
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call setjmp wrt ..plt
	test eax, eax
	je %%skip

	dec eax
	push rax
	mov edi, eax
	sub rsp, byte 0x8
	call grug_get_runtime_error_reason wrt ..plt
	add rsp, byte 0x8
	mov rdi, rax

	lea rcx, [rel init_globals_fn_path]

	lea rdx, [rel init_globals_fn_name]

	pop rsi

	mov rax, [rel grug_runtime_error_handler wrt ..got]
	call [rax]

	mov rsp, rbp
	pop rbp
	ret
%%skip:
%endmacro

%macro on_fn_error_handling 0
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call setjmp wrt ..plt
	test eax, eax
	je %%skip

	dec eax
	push rax
	mov edi, eax
	sub rsp, byte 0x8
	call grug_get_runtime_error_reason wrt ..plt
	add rsp, byte 0x8
	mov rdi, rax

	lea rcx, [rel on_fn_path]

	lea rdx, [rel on_fn_name]

	pop rsi

	mov rax, [rel grug_runtime_error_handler wrt ..got]
	call [rax]

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
	mov rsi, [rel grug_max_time wrt ..got]
	push rsi
	mov edi, CLOCK_PROCESS_CPUTIME_ID
	; Fill grug_max_time with the current time.
	call clock_gettime wrt ..plt
	pop rax
	; grug_max_time.nsec += GRUG_ON_FN_TIME_LIMIT_MS * NS_PER_MS;
	add qword [byte rax + TV_NSEC_OFFSET], GRUG_ON_FN_TIME_LIMIT_MS * NS_PER_MS
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
	mov esi, 1 + GRUG_ON_FN_STACK_OVERFLOW
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call longjmp wrt ..plt
%%skip:
%endmacro

%macro check_time_limit_exceeded 0
	mov rsi, [rel grug_current_time wrt ..got]
	push rsi
	mov edi, CLOCK_PROCESS_CPUTIME_ID
	; Fill grug_current_time with the current time.
	call clock_gettime wrt ..plt
	pop rax
	mov r11, [rel grug_max_time wrt ..got]

	; if (grug_current_time.sec < grug_max_time.sec) goto skip;
	mov r10, [byte r11 + TV_SEC_OFFSET]
	cmp [byte rax + TV_SEC_OFFSET], r10
	jl %%skip
	; if (grug_current_time.sec > grug_max_time.sec) goto longjmp;
	jg %%longjump
	; if (grug_current_time.nsec > grug_max_time.nsec) goto longjmp;
	mov r10, [byte r11 + TV_NSEC_OFFSET]
	cmp [byte rax + TV_NSEC_OFFSET], r10
	jg %%longjump
	; goto skip;
	jmp short %%skip
%%longjump:
	; longjmp(grug_runtime_error_jmp_buffer, 1 + GRUG_ON_FN_TIME_LIMIT_EXCEEDED);
	mov esi, 1 + GRUG_ON_FN_TIME_LIMIT_EXCEEDED
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call longjmp wrt ..plt
%%skip:
%endmacro

%macro check_division_overflow 0
	cmp eax, -2147483648
	jne %%skip
	cmp r11d, -1
	jne %%skip
	mov esi, 1 + GRUG_ON_FN_OVERFLOW
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call longjmp wrt ..plt
%%skip:
%endmacro

%macro check_division_by_0 0
	test r11, r11
	jne %%skip
	mov esi, 1 + GRUG_ON_FN_DIVISION_BY_ZERO
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call longjmp wrt ..plt
%%skip:
%endmacro

%macro check_overflow 0
	jno %%skip
	mov esi, 1 + GRUG_ON_FN_OVERFLOW
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call longjmp wrt ..plt
%%skip:
%endmacro
