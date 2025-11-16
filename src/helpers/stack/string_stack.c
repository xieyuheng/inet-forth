#include "index.h"

stack_t *
string_make_stack(void) {
    stack_t *self = make_stack_with((destroy_fn_t *) string_destroy);
    return self;
}
