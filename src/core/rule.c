#include "index.h"

rule_t *
rule_new(
    const node_ctor_t *first_node_ctor,
    const node_ctor_t *second_node_ctor,
    const function_t *function
) {
    rule_t *self = new(rule_t);
    self->first_node_ctor = first_node_ctor;
    self->second_node_ctor = second_node_ctor;
    self->function = function;
    return self;
}

void
rule_destroy(rule_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    rule_t *self = *self_pointer;
    // NOTE A rule dose not own `function`.
    //   because they might be shared.
    // function_destroy(&self->function);
    free(self);
    *self_pointer = NULL;
}

void
rule_print(const rule_t *self, file_t *file) {
    fprintf(file, "! (%s)-(%s) ",
            self->first_node_ctor->name,
            self->second_node_ctor->name);
    function_print(self->function, file);
}
