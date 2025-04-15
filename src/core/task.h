#pragma once

struct task_t {
    principal_wire_t *left;
    principal_wire_t *right;
    const rule_t *rule;
#if DEBUG_TASK_MUTEX
    mutex_t *mutex;
#endif
};

task_t *task_new(principal_wire_t *left, principal_wire_t *right, const rule_t *rule);
void task_destroy(task_t **self_pointer);

void task_print(const task_t *self, file_t *file);
