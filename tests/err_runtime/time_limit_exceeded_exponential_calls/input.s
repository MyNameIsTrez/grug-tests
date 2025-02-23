section .data

global define_type
define_type: db "d", 0

align 8
global globals_size
globals_size: dq 8

global on_fns
on_fns:
	dq on_a

on_fn_path:
	db "tests/err_runtime/time_limit_exceeded_exponential_calls/input.grug", 0
on_fn_name:
	db "on_a", 0

align 8
global resources_size
resources_size: dq 0

global entities_size
entities_size: dq 0

section .text

extern grug_runtime_error_handler
extern grug_max_rsp
extern grug_max_time
extern grug_on_fn_name
extern grug_runtime_error_jmp_buffer
extern grug_on_fn_path
extern grug_on_fns_in_safe_mode
extern grug_current_time
extern game_fn_define_d
extern clock_gettime
extern setjmp
extern grug_get_runtime_error_reason
extern longjmp
extern game_fn_nothing

%define GRUG_ON_FN_STACK_OVERFLOW 1
%define GRUG_ON_FN_TIME_LIMIT_EXCEEDED 2

%define GRUG_STACK_LIMIT 0x10000
%define CLOCK_PROCESS_CPUTIME_ID 2
%define TV_SEC_OFFSET 0
%define TV_NSEC_OFFSET 8
%define GRUG_ON_FN_TIME_LIMIT_MS 10
%define NS_PER_MS 1000000
%define NS_PER_SEC 1000000000

global define
define:
	sub rsp, byte 0x8
	call game_fn_define_d wrt ..plt
	add rsp, byte 0x8
	ret

global init_globals
init_globals:
	mov rdi[0x0], rsi
	ret

%macro save_on_fn_name_and_path 0
	mov rax, [rel grug_on_fn_path wrt ..got]
	lea r11, [rel on_fn_path]
	mov [rax], r11

	mov rax, [rel grug_on_fn_name wrt ..got]
	lea r11, [rel on_fn_name]
	mov [rax], r11
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
	call clock_gettime wrt ..plt
	pop rax
	add qword [byte rax + TV_NSEC_OFFSET], GRUG_ON_FN_TIME_LIMIT_MS * NS_PER_MS
	cmp qword [byte rax + TV_NSEC_OFFSET], NS_PER_SEC
	jl %%skip
	sub qword [byte rax + TV_NSEC_OFFSET], NS_PER_SEC
	inc qword [byte rax + TV_SEC_OFFSET]
%%skip:
%endmacro

%macro error_handling 0
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
	call clock_gettime wrt ..plt
	pop rax
	mov r11, [rel grug_max_time wrt ..got]

	mov r10, [byte r11 + TV_SEC_OFFSET]
	cmp [byte rax + TV_SEC_OFFSET], r10
	jl %%skip
	jg %%longjump
	mov r10, [byte r11 + TV_NSEC_OFFSET]
	cmp [byte rax + TV_NSEC_OFFSET], r10
	jg %%longjump
	jmp short %%skip
%%longjump:
	mov esi, 1 + GRUG_ON_FN_TIME_LIMIT_EXCEEDED
	mov rdi, [rel grug_runtime_error_jmp_buffer wrt ..got]
	call longjmp wrt ..plt
%%skip:
%endmacro

global on_a
on_a:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, [rel grug_on_fns_in_safe_mode wrt ..got]
	mov al, [rax]
	test al, al
	je strict .fast

	save_on_fn_name_and_path

	set_max_rsp

	set_time_limit

	error_handling

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_a_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_a_safe

	mov rsp, rbp
	pop rbp
	ret

.fast:
	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_a_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_a_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_a_safe
helper_a_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_b_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_b_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_a_fast
helper_a_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_b_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_b_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_b_safe
helper_b_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_c_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_c_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_b_fast
helper_b_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_c_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_c_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_c_safe
helper_c_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_d_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_d_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_c_fast
helper_c_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_d_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_d_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_d_safe
helper_d_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_e_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_e_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_d_fast
helper_d_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_e_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_e_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_e_safe
helper_e_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_f_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_f_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_e_fast
helper_e_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_f_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_f_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_f_safe
helper_f_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_g_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_g_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_f_fast
helper_f_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_g_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_g_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_g_safe
helper_g_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_h_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_h_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_g_fast
helper_g_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_h_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_h_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_h_safe
helper_h_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_i_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_i_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_h_fast
helper_h_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_i_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_i_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_i_safe
helper_i_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_j_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_j_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_i_fast
helper_i_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_j_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_j_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_j_safe
helper_j_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_k_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_k_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_j_fast
helper_j_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_k_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_k_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_k_safe
helper_k_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_l_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_l_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_k_fast
helper_k_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_l_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_l_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_l_safe
helper_l_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_m_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_m_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_l_fast
helper_l_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_m_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_m_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_m_safe
helper_m_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_n_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_n_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_m_fast
helper_m_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_n_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_n_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_n_safe
helper_n_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_o_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_o_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_n_fast
helper_n_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_o_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_o_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_o_safe
helper_o_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_p_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_p_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_o_fast
helper_o_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_p_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_p_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_p_safe
helper_p_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_q_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_q_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_p_fast
helper_p_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_q_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_q_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_q_safe
helper_q_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_r_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_r_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_q_fast
helper_q_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_r_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_r_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_r_safe
helper_r_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_s_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_s_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_r_fast
helper_r_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_s_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_s_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_s_safe
helper_s_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_t_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_t_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_s_fast
helper_s_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_t_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_t_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_t_safe
helper_t_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_u_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_u_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_t_fast
helper_t_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_u_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_u_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_u_safe
helper_u_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_v_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_v_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_u_fast
helper_u_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_v_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_v_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_v_safe
helper_v_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_w_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_w_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_v_fast
helper_v_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_w_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_w_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_w_safe
helper_w_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_x_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_x_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_w_fast
helper_w_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_x_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_x_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_x_safe
helper_x_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_y_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_y_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_x_fast
helper_x_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_y_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_y_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_y_safe
helper_y_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_z_safe

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_z_safe

	mov rsp, rbp
	pop rbp
	ret

global helper_y_fast
helper_y_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_z_fast

	mov rax, rbp[-0x8]
	push rax
	pop rdi
	call helper_z_fast

	mov rsp, rbp
	pop rbp
	ret

global helper_z_safe
helper_z_safe:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi
	check_stack_overflow
	check_time_limit_exceeded

	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret

global helper_z_fast
helper_z_fast:
	push rbp
	mov rbp, rsp
	sub rsp, byte 0x10
	mov rbp[-0x8], rdi

	call game_fn_nothing wrt ..plt

	mov rsp, rbp
	pop rbp
	ret
