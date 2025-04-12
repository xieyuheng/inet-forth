#pragma once

struct task_t {
    principal_wire_t *left;
    principal_wire_t *right;
    const rule_t *rule;
};

task_t *task_new(principal_wire_t *left, principal_wire_t *right, const rule_t *rule);
void task_destroy(task_t **self_pointer);
