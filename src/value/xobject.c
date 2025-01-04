#include "index.h"

bool
is_xobject(value_t value) {
    return value_tag(value) == XOBJECT;
}
