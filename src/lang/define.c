#include "index.h"

void
define(mod_t *self, const char *name, value_t value) {
    assert(hash_set(self->value_hash, string_copy(name), value));
}

void
define_function(mod_t *mod, const char *name, function_t *function) {
    function->name = string_copy(name);
    define(mod, name, function);
}

void
define_primitive_fn(mod_t *mod, const char *name, size_t input_arity, size_t output_arity, primitive_fn_t *primitive_fn) {
    define(mod, name, primitive_from_fn(name, input_arity, output_arity, primitive_fn));
}

void
define_primitive_fn_0(mod_t *mod, const char *name, primitive_fn_0_t *primitive_fn_0) {
    define(mod, name, primitive_from_fn_0(name, primitive_fn_0));
}

void
define_primitive_fn_1(mod_t *mod, const char *name, primitive_fn_1_t *primitive_fn_1) {
    define(mod, name, primitive_from_fn_1(name, primitive_fn_1));
}

void
define_primitive_fn_2(mod_t *mod, const char *name, primitive_fn_2_t *primitive_fn_2) {
    define(mod, name, primitive_from_fn_2(name, primitive_fn_2));
}

void
define_primitive_fn_3(mod_t *mod, const char *name, primitive_fn_3_t *primitive_fn_3) {
    define(mod, name, primitive_from_fn_3(name, primitive_fn_3));
}

void
define_primitive_fn_4(mod_t *mod, const char *name, primitive_fn_4_t *primitive_fn_4)  {
    define(mod, name, primitive_from_fn_4(name, primitive_fn_4));
}
