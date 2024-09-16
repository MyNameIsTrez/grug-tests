#pragma once

#include <stdint.h>

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
void game_fn_define_t(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8);
void game_fn_define_u(char *sprite_path);
void game_fn_define_v(char *foo, char *bar);
void game_fn_define_w(char *sprite_path);
void game_fn_define_x(char *projectile);
void game_fn_define_y(char *foo, char *bar);
void game_fn_define_z(char *projectile);
void game_fn_define_a2(char *sprite_path, char *projectile);
void game_fn_define_b2(char *sprite_path, char *projectile);

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
void w_on_a(void *globals, int32_t self);
void z_on_a(void *globals, int32_t self);
void a2_on_a(void *globals, int32_t self);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlanguage-extension-token"
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
struct w_on_fns {
    typeof(w_on_a) *a;
};
struct z_on_fns {
    typeof(z_on_a) *a;
};
struct a2_on_fns {
    typeof(a2_on_a) *a;
};
#pragma GCC diagnostic pop

void game_fn_nothing(void);
int32_t game_fn_magic(void);
void game_fn_initialize(int32_t x);
void game_fn_initialize_bool(bool b);
int32_t game_fn_identity(int32_t x);
int32_t game_fn_max(int32_t x, int32_t y);
void game_fn_say(char *message);
float game_fn_sin(float x);
float game_fn_cos(float x);
void game_fn_mega(float f1, int32_t i1, bool b1, float f2, float f3, float f4, bool b2, int32_t i2, float f5, float f6, float f7, float f8, int32_t i3, char *str);
bool game_fn_is_friday(void);
void game_fn_set_is_happy(bool is_happy);
void game_fn_mega_f32(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9);
void game_fn_mega_i32(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7);
// void game_fn_draw(sprite_path);
void game_fn_blocked_alrm(void);
void game_fn_nothing_aligned(void);
int32_t game_fn_magic_aligned(void);
void game_fn_initialize_aligned(int32_t x);
