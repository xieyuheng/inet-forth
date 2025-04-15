#pragma once

struct rule_t {
    const node_ctor_t *left_node_ctor;
    const node_ctor_t *right_node_ctor;
    const function_t *function;
};

rule_t *rule_new(
    const node_ctor_t *left_node_ctor,
    const node_ctor_t *right_node_ctor,
    const function_t *function);
void rule_destroy(rule_t **self_pointer);

void rule_print(const rule_t *self, file_t *file);
