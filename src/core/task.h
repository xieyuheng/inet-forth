#pragma once

struct task_t {
    wire_t *wire;
    const rule_t *rule;
};

task_t *task_new(wire_t *wire, const rule_t *rule);
void task_destroy(task_t **self_pointer);
