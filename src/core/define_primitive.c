#include "index.h"

void
define_primitive_fn(mod_t *mod, const char *name, primitive_fn_t *primitive_fn) {
    mod_define(mod, def_primitive(primitive_from_fn(name, primitive_fn)));
}

void
define_primitive_fn_0(mod_t *mod, const char *name, primitive_fn_0_t *primitive_fn_0) {
    mod_define(mod, def_primitive(primitive_from_fn_0(name, primitive_fn_0)));
}

void
define_primitive_fn_1(mod_t *mod, const char *name, primitive_fn_1_t *primitive_fn_1) {
    mod_define(mod, def_primitive(primitive_from_fn_1(name, primitive_fn_1)));
}

void
define_primitive_fn_2(mod_t *mod, const char *name, primitive_fn_2_t *primitive_fn_2) {
    mod_define(mod, def_primitive(primitive_from_fn_2(name, primitive_fn_2)));
}

void
define_primitive_fn_3(mod_t *mod, const char *name, primitive_fn_3_t *primitive_fn_3) {
    mod_define(mod, def_primitive(primitive_from_fn_3(name, primitive_fn_3)));
}

void
define_primitive_fn_4(mod_t *mod, const char *name, primitive_fn_4_t *primitive_fn_4)  {
    mod_define(mod, def_primitive(primitive_from_fn_4(name, primitive_fn_4)));
}
