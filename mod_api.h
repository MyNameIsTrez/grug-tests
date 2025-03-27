#pragma once

#include <stdbool.h>
#include <stdint.h>

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
void game_fn_draw(char *sprite_path);
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
void game_fn_offset_32_bit_f32(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7, char *s8, char *s9, char *s10, char *s11, char *s12, char *s13, char *s14, char *s15, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, int32_t g);
void game_fn_offset_32_bit_i32(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, int32_t i1, int32_t i2, int32_t i3, int32_t i4, int32_t i5, int32_t g);
void game_fn_offset_32_bit_string(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, char *s1, char *s2, char *s3, char *s4, char *s5, int32_t g);
void game_fn_talk(char *message1, char *message2, char *message3, char *message4);
uint64_t game_fn_get_position(uint64_t id);
