#include "index.h"

task_t *
task_new(wire_t *wire, const rule_t *rule) {
    task_t *self = new(task_t);
    self->wire = wire;
    self->rule = rule;
    return self;
}

void
task_destroy(task_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    task_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}
