#pragma once

#include <stdint.h>

struct b {
    int32_t x;
};
struct c {
    int32_t x;
    int32_t y;
};
struct h {
    int32_t x;
};
struct i {
    int32_t x;
    int32_t y;
};
struct k {
    int32_t age;
    char *name;
};
struct l {
    char *group;
    char *name;
};
struct m {
    int32_t w;
    char *group;
    bool b1;
    char *name;
    bool b2;
    int32_t z;
};
struct n {
    int32_t u;
    int32_t v;
    int32_t w;
    int32_t x;
    int32_t y;
    int32_t z;
};
struct o {
    char *u;
    char *v;
    char *w;
    char *x;
    char *y;
    char *z;
};
struct p {
    char *x;
};
struct q {
    char *a;
    char *b;
    char *c;
};

void game_fn_define_a(void);
void game_fn_define_b(int32_t x);
void game_fn_define_c(int32_t x, int32_t y);
void game_fn_define_d(void);
void game_fn_define_e(void);
void game_fn_define_f(void);
void game_fn_define_g(void);
void game_fn_define_h(int32_t x);
void game_fn_define_i(int32_t x, int32_t y);
void game_fn_define_j(void);
void game_fn_define_k(int32_t age, char *name);
void game_fn_define_l(char *group, char *name);
void game_fn_define_m(int32_t w, char *group, bool b1, char *name, bool b2, int32_t z);
void game_fn_define_n(int32_t u, int32_t v, int32_t w, int32_t x, int32_t y, int32_t z);
void game_fn_define_o(char *u, char *v, char *w, char *x, char *y, char *z);
void game_fn_define_p(char *x);
void game_fn_define_q(char *a, char *b, char *c);
void game_fn_define_r(void);
void game_fn_define_s(void);

void d_on_a(void *globals);
void e_on_a(void *globals);
void e_on_b(void *globals);
void f_on_a(void *globals, int32_t self);
void g_on_a(void *globals, int32_t self, int32_t x);
void h_on_a(void *globals, int32_t self);
void i_on_a(void *globals, int32_t self);
void i_on_b(void *globals, int32_t self, int32_t x);
void j_on_a(void *globals);
void j_on_b(void *globals);
void j_on_c(void *globals);
void p_on_a(void *globals);
void r_on_a(void *globals, float f);
void s_on_a(void *globals, int32_t i, float f);

struct d_on_fns {
    typeof(d_on_a) *a;
};
struct e_on_fns {
    typeof(e_on_a) *a;
    typeof(e_on_b) *b;
};
struct f_on_fns {
    typeof(f_on_a) *a;
};
struct g_on_fns {
    typeof(g_on_a) *a;
};
struct h_on_fns {
    typeof(h_on_a) *a;
};
struct i_on_fns {
    typeof(i_on_a) *a;
    typeof(i_on_b) *b;
};
struct j_on_fns {
    typeof(j_on_a) *a;
    typeof(j_on_b) *b;
    typeof(j_on_c) *c;
};
struct p_on_fns {
    typeof(p_on_a) *a;
};
struct r_on_fns {
    typeof(r_on_a) *a;
};
struct s_on_fns {
    typeof(s_on_a) *a;
};
