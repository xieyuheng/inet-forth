#include "index.h"

rule_t *
rule_new(
    const node_ctor_t *first_node_ctor,
    const node_ctor_t *second_node_ctor,
    function_t *function
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
    if (*self_pointer) {
        rule_t *self = *self_pointer;
        function_destroy(&self->function);
        free(self);
        *self_pointer = NULL;
    }
}

bool
rule_match_wire_pair(
    const rule_t *self,
    const wire_t *first_wire,
    const wire_t *second_wire
) {
    return (((self->first_node_ctor == first_wire->node->def) &&
             (self->second_node_ctor == second_wire->node->def)) ||
            ((self->first_node_ctor == second_wire->node->def) &&
             (self->second_node_ctor == first_wire->node->def)));
}

void
rule_print(const rule_t *self, file_t *file) {
    fprintf(file, "! (%s)-(%s) ",
            self->first_node_ctor->name,
            self->second_node_ctor->name);
    function_print(self->function, file);
}
