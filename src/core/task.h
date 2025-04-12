#pragma once

struct task_t {
    principal_port_t *left_principal_port;
    principal_port_t *right_principal_port;
    const rule_t *rule;
};

task_t *task_new(principal_port_t *left_principal_port, principal_port_t *right_principal_port, const rule_t *rule);
void task_destroy(task_t **self_pointer);
