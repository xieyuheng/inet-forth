#include "index.h"

stack_t *
string_stack_new(void) {
    stack_t *self = stack_new_with((destroy_fn_t *) string_destroy);
    return self;
}
