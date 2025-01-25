#include "index.h"

port_info_t *
port_info_new(const char *name, bool is_principal) {
    port_info_t *self = new(port_info_t);
    self->name = string_copy(name);
    self->is_principal = is_principal;
    return self;
}

void
port_info_destroy(port_info_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        port_info_t *self = *self_pointer;
        free(self->name);
        free(self);
        *self_pointer = NULL;
    }
}


port_info_t *
port_info_from_name(const char *name) {
    if (string_ends_with(name, "!")) {
        name = string_slice(name, 0, strlen(name) - 1);
        return port_info_new(name, true);
    } else {
        return port_info_new(name, false);
    }
}
