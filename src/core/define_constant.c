#include "index.h"

void
define_constant(mod_t *mod, const char *name, value_t value) {
    mod_define(mod, def_constant(constant_def_new(name, value)));
}
