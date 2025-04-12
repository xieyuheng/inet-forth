#include "index.h"

task_t *
task_new(principal_port_t *left_principal_port, principal_port_t *right_principal_port, const rule_t *rule) {
    task_t *self = new(task_t);
    self->left_principal_port = left_principal_port;
    self->right_principal_port = right_principal_port;
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
