#include "index.h"

mod_t *
mod_new(const char *src, const char *code) {
    mod_t *self = new(mod_t);
    self->src = src;
    self->code = code;
    self->def_hash = hash_new_of_string_key();
    hash_set_destroy_fn(self->def_hash, (destroy_fn_t *) def_destroy);
    self->rule_list = list_new_with((destroy_fn_t *) rule_destroy);
    return self;
}

void
mod_destroy(mod_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        mod_t *self = *self_pointer;
        hash_destroy(&self->def_hash);
        list_destroy(&self->rule_list);
        free(self);
        *self_pointer = NULL;
    }
}

const def_t *
mod_find_def(const mod_t *self, const char *name) {
    return hash_get(self->def_hash, name);
}

const rule_t *
mod_find_rule(
    const mod_t *self,
    const wire_t *first_wire,
    const wire_t *second_wire
) {
    rule_t *rule = list_first(self->rule_list);
    while (rule) {
        if (rule_match_wire_pair(rule, first_wire, second_wire)) return rule;
        rule = list_next(self->rule_list);
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
    const def_t *first_def = mod_find_def(self, first_name);
    const def_t *second_def = mod_find_def(self, second_name);

    const node_ctor_t *first_node_ctor = first_def->as_node_ctor;
    const node_ctor_t *second_node_ctor = second_def->as_node_ctor;

    rule_t *rule = rule_new(first_node_ctor, second_node_ctor, function);
    list_push(self->rule_list, rule);
}

void
mod_print(const mod_t *self, file_t *file) {
    fprintf(file, "<mod def-count=\"%lu\" rule-count=\"%lu\">\n",
            hash_length(self->def_hash),
            list_length(self->rule_list));

    def_t *def = hash_first(self->def_hash);
    while (def) {
        def_print(def, file);
        fprintf(file, "\n");
        def = hash_next(self->def_hash);
    }

    rule_t *rule = list_first(self->rule_list);
    while (rule) {
        rule_print(rule, file);
        fprintf(file, "\n");
        rule = list_next(self->rule_list);
    }

    fprintf(file, "</mod>\n");
}
