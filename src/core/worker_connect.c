#include "index.h"

static void
worker_add_task(worker_t *self, task_t *task) {
    queue_back_push(self->task_queue, task);
    if (self->scheduler) {
        atomic_add1(&self->scheduler->atomic_task_count);
    }
}


inline static task_t *
rule_match(const rule_t *rule, principal_wire_t *left, principal_wire_t *right) {
    if ((rule->left_node_ctor == left->node->ctor) &&
        (rule->right_node_ctor == right->node->ctor)) {
        return task_new(left, right, rule);
    } else if ((rule->left_node_ctor == right->node->ctor) &&
               (rule->right_node_ctor == left->node->ctor)) {
        return task_new(right, left, rule);
    } else {
        return NULL;
    }
}

static void
worker_connect_active_pair(worker_t *self, principal_wire_t *left, principal_wire_t *right) {
    as_principal_wire(left)->oppsite = as_principal_wire(right);
    as_principal_wire(right)->oppsite = as_principal_wire(left);

    for (size_t i = 0; i < left->node->ctor->arity; i++) {
        rule_t *rule = array_get(left->node->ctor->rule_array, i);
        task_t* task = rule_match(rule, left, right);
        if (task) {
            worker_add_task(self, task);
            return;
        }
    }

    for (size_t i = 0; i < right->node->ctor->arity; i++) {
        rule_t *rule = array_get(right->node->ctor->rule_array, i);
        task_t* task = rule_match(rule, left, right);
        if (task) {
            worker_add_task(self, task);
            return;
        }
    }
}

static void
worker_fuze(worker_t *self, wire_t *wire, value_t value) {
    value_t fuzed_value = NULL;
    if (!atomic_compare_exchange_strong(
            &wire->atomic_fuzed_value,
            &fuzed_value,
            value))
    {
        wire_destroy(&wire);
        worker_connect(self, fuzed_value, value);
    }
}

void
worker_connect(worker_t *self, value_t left, value_t right) {
    if (is_principal_wire(left) && is_principal_wire(right)) {
        worker_connect_active_pair(self, as_principal_wire(left), as_principal_wire(right));
    } else if (is_wire(left)) {
        worker_fuze(self, as_wire(left), right);
    } else if (is_wire(right)) {
        worker_fuze(self, as_wire(right), left);
    } else {
        assert(false);
    }
}
