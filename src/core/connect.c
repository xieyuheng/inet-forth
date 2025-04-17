#include "index.h"

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

inline static task_t *
connect_active_pair(principal_wire_t *left, principal_wire_t *right) {
    as_principal_wire(left)->oppsite = as_principal_wire(right);
    as_principal_wire(right)->oppsite = as_principal_wire(left);

    for (size_t i = 0; i < left->node->ctor->arity; i++) {
        rule_t *rule = array_get(left->node->ctor->rule_array, i);
        task_t* task = rule_match(rule, left, right);
        if (task) {
            return task;
        }
    }

    for (size_t i = 0; i < right->node->ctor->arity; i++) {
        rule_t *rule = array_get(right->node->ctor->rule_array, i);
        task_t* task = rule_match(rule, left, right);
        if (task) {
            return task;
        }
    }

    return NULL;
}

inline static task_t *
fuze(wire_t *wire, value_t value) {
    // expected `wire->atomic_fuzed_value` to be NULL
    value_t expected_fuzed_value = NULL;
    if (atomic_compare_exchange_strong(
            &wire->atomic_fuzed_value,
            &expected_fuzed_value,
            value)) {
        return NULL;
    } {
        // `expected_fuzed_value` now stores
        // the unexpected value in `wire->atomic_fuzed_value`
        wire_destroy(&wire);
        return connect(expected_fuzed_value, value);
    }
}

task_t *
connect(value_t left, value_t right) {
    if (is_principal_wire(left) && is_principal_wire(right)) {
        return connect_active_pair(as_principal_wire(left), as_principal_wire(right));
    } else if (is_wire(left)) {
        return fuze(as_wire(left), right);
    } else if (is_wire(right)) {
        return fuze(as_wire(right), left);
    } else {
        file_lock(stdout);
        test_printf("impossible case!\n");
        test_printf("left: "); value_print(left, stdout); printf("\n");
        test_printf("right: "); value_print(right, stdout); printf("\n");
        file_unlock(stdout);
        assert(false);
    }
}
