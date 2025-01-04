#include "index.h"

activity_t *
activity_new(wire_t *wire, const rule_t *rule) {
    activity_t *self = new(activity_t);
    self->wire = wire;
    self->rule = rule;
    return self;
}
