#include "index.h"

mod_t *
mod_new(const char *src, const char *code) {
    mod_t *self = new(mod_t);
    self->src = src;
    self->code = code;
    self->def_hash = hash_of_string_key();
    hash_set_destroy_fn(self->def_hash, (destroy_fn_t *) def_destroy);
    return self;
}

void
mod_destroy(mod_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    mod_t *self = *self_pointer;
    hash_destroy(&self->def_hash);
    free(self);
    *self_pointer = NULL;
}

const def_t *
mod_find(const mod_t *self, const char *name) {
    return hash_get(self->def_hash, name);
}

static bool
rule_match(const rule_t *self, const node_t *first_node, const node_t *second_node) {
    return (((self->first_node_ctor == first_node->ctor) &&
             (self->second_node_ctor == second_node->ctor)) ||
            ((self->first_node_ctor == second_node->ctor) &&
             (self->second_node_ctor == first_node->ctor)));
}

const rule_t *
mod_find_rule(const mod_t *self, const principal_wire_t *left, const principal_wire_t *right) {
    (void) self;

    for (size_t i = 0; i < left->node->ctor->arity; i++) {
        rule_t *rule = array_get(left->node->ctor->rule_array, i);
        if (rule_match(rule, left->node, right->node))
            return rule;
    }

    for (size_t i = 0; i < right->node->ctor->arity; i++) {
        rule_t *rule = array_get(right->node->ctor->rule_array, i);
        if (rule_match(rule, left->node, right->node))
            return rule;
    }

    return NULL;
}

void
mod_define(mod_t *self, def_t *def) {
    assert(hash_set(self->def_hash, string_copy(def_name(def)), def));
}

void
mod_define_rule(
    mod_t *self,
    const char *first_name,
    const char *second_name,
    function_t *function
) {
    const def_t *first_def = mod_find(self, first_name);
    const def_t *second_def = mod_find(self, second_name);

    assert(first_def->kind == DEF_NODE_CTOR);
    assert(second_def->kind == DEF_NODE_CTOR);

    const node_ctor_t *first_node_ctor = first_def->node_ctor;
    const node_ctor_t *second_node_ctor = second_def->node_ctor;

    rule_t *rule = rule_new(first_node_ctor, second_node_ctor, function);

    if (first_node_ctor == second_node_ctor) {
        array_push(first_node_ctor->rule_array, rule);
    } else {
        array_push(first_node_ctor->rule_array, rule);
        array_push(second_node_ctor->rule_array, rule);
    }
}

void
mod_print(const mod_t *self, file_t *file) {
    fprintf(file, "<mod def-count=\"%lu\"\n", hash_length(self->def_hash));

    def_t *def = hash_first(self->def_hash);
    while (def) {
        def_print(def, file);
        fprintf(file, "\n");
        def = hash_next(self->def_hash);
    }

    fprintf(file, "</mod>\n");
}
