#include "index.h"

void
define_function(mod_t *mod, const char *name, function_t *function) {
    function->name = string_copy(name);
    mod_define(mod, def_function(function));
}
