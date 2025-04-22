#pragma once

void define(mod_t *self, const char *name, value_t value);
void define_constant(mod_t *mod, const char *name, value_t value);
void define_function(mod_t *mod, const char *name, function_t *function);
void define_primitive_fn(mod_t *mod, const char *name, size_t input_arity, size_t output_arity, primitive_fn_t *primitive_fn);
void define_primitive_fn_0(mod_t *mod, const char *name, primitive_fn_0_t *primitive_fn_0);
void define_primitive_fn_1(mod_t *mod, const char *name, primitive_fn_1_t *primitive_fn_1);
void define_primitive_fn_2(mod_t *mod, const char *name, primitive_fn_2_t *primitive_fn_2);
void define_primitive_fn_3(mod_t *mod, const char *name, primitive_fn_3_t *primitive_fn_3);
void define_primitive_fn_4(mod_t *mod, const char *name, primitive_fn_4_t *primitive_fn_4);
