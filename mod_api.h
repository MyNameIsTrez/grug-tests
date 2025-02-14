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
void game_fn_define_c2(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, int32_t i8, float f10);
void game_fn_define_d2(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, int32_t i8, float f9);

struct d_on_fns {
    void (*a)(void *globals);
};
struct e_on_fns {
    void (*a)(void *globals);
    void (*b)(void *globals);
};
struct f_on_fns {
    void (*a)(void *globals, int32_t x);
};
struct g_on_fns {
    void (*a)(void *globals, int32_t x, int32_t y);
};
struct h_on_fns {
    void (*a)(void *globals, int32_t x);
};
struct i_on_fns {
    void (*a)(void *globals, int32_t x);
    void (*b)(void *globals, int32_t x, int32_t y);
};
struct j_on_fns {
    void (*a)(void *globals);
    void (*b)(void *globals);
    void (*c)(void *globals);
};
struct p_on_fns {
    void (*a)(void *globals);
};
struct r_on_fns {
    void (*a)(void *globals, float f);
};
struct s_on_fns {
    void (*a)(void *globals, int32_t i, float f);
};
struct w_on_fns {
    void (*a)(void *globals, int32_t x);
};
struct z_on_fns {
    void (*a)(void *globals, int32_t x);
};
struct a2_on_fns {
    void (*a)(void *globals, int32_t x);
};

void game_fn_nothing(void);
int32_t game_fn_magic(void);
void game_fn_initialize(int32_t x);
void game_fn_initialize_bool(bool b);
int32_t game_fn_identity(int32_t x);
int32_t game_fn_max(int32_t x, int32_t y);
void game_fn_say(char *message);
float game_fn_sin(float x);
float game_fn_cos(float x);
void game_fn_mega(float f1, int32_t i1, bool b1, float f2, float f3, float f4, bool b2, int32_t i2, float f5, float f6, float f7, float f8, uint64_t id, char *str);
bool game_fn_is_friday(void);
void game_fn_set_is_happy(bool is_happy);
void game_fn_mega_f32(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9);
void game_fn_mega_i32(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7);
void game_fn_blocked_alrm(void);
void game_fn_nothing_aligned(void);
int32_t game_fn_magic_aligned(void);
void game_fn_initialize_aligned(int32_t x);
void game_fn_spawn(char *name);
bool game_fn_has_resource(char *path);
bool game_fn_has_entity(char *name);
bool game_fn_has_string(char *str);
uint64_t game_fn_get_opponent(void);
void game_fn_set_target(uint64_t target);
void game_fn_motherload(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, uint64_t id, float f9);
void game_fn_motherload_subless(int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t i6, int32_t i7, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, uint64_t id, float f10);
void game_fn_offset_32_bit_f32(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7, char *s8, char *s9, char *s10, char *s11, char *s12, char *s13, char *s14, char *s15, float f1, int32_t g);
void game_fn_offset_32_bit_i32_and_string(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, int32_t i1, char *s1, int32_t g);
