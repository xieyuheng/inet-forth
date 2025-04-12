#include "index.h"

task_t *
task_new(principal_wire_t *left, principal_wire_t *right, const rule_t *rule) {
    task_t *self = new(task_t);
    self->left = left;
    self->right = right;
    self->rule = rule;
    return self;
}

void
task_destroy(task_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    task_t *self = *self_pointer;
    principal_wire_destroy(&self->left);
    principal_wire_destroy(&self->right);
    free(self);
    *self_pointer = NULL;
}
