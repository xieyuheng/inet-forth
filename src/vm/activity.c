#include "index.h"

activity_t *
activity_new(wire_t *wire, const rule_t *rule) {
    activity_t *self = new(activity_t);
    self->wire = wire;
    self->rule = rule;
    return self;
}

void
activity_destroy(activity_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        activity_t *self = *self_pointer;
        free(self);
        *self_pointer = NULL;
    }
}
