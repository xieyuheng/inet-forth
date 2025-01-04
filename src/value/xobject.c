#include "index.h"

value_t
xobject(object_t *target) {
    return target;
}

bool
is_xobject(value_t value) {
    return value_tag(value) == XOBJECT;
}

object_t *
to_object(value_t value) {
    assert(is_xobject(value));
    return value;
}
