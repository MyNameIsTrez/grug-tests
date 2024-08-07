#pragma once

#include <stdint.h>

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
