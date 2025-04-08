#include "index.h"

void
node_init(node_t *self) {
    self->value_array = array_auto();
}

node_t *
node_new(const node_ctor_t *ctor, size_t id) {
    node_t *self = new(node_t);
    self->ctor = ctor;
    self->id = id;
    node_init(self);
    return self;
}

void
node_destroy(node_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        node_t *self = *self_pointer;
        array_destroy(&self->value_array);
        free(self);
        *self_pointer = NULL;
    }
}

void
node_set(node_t *self, size_t index, value_t value) {
    assert(index < self->ctor->arity);
    array_set(self->value_array, index, value);

    if (is_wire(value)) {
        wire_t *wire = as_wire(value);
        assert(wire->node == NULL);
        wire->node = self;
        wire->index = index;

        port_info_t *port_info = self->ctor->port_infos[index];
        if (port_info->is_principal) {
            atomic_store(&wire->atomic_is_principal, true);
        }
    }
}

value_t node_get(const node_t *self, size_t index) {
    assert(index < self->ctor->arity);
    return array_get(self->value_array, index);
}

void
node_print(const node_t *self, file_t *file) {
    char *id_string = uint_to_subscript(self->id);
    fprintf(file, "(%s%s)", self->ctor->name, id_string);
    free(id_string);
}
