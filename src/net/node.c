#include "index.h"

node_t *
node_new(const node_ctor_t *ctor) {
    node_t *self = new(node_t);
    self->ctor = ctor;
    self->value_array = array_auto();
    return self;
}

void
node_destroy(node_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}

node_t *
node_new_per_thread(node_allocator_t *node_allocator, stack_t *free_node_stack, const node_ctor_t *ctor) {
    node_t *self = node_allocator_allocate(node_allocator, free_node_stack);
    self->ctor = ctor;
    return self;
}

void
node_recycle_per_thread(node_allocator_t *node_allocator, stack_t *free_node_stack, node_t **self_pointer) {
    assert(self_pointer);
    node_allocator_recycle(node_allocator, free_node_stack, self_pointer);
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

value_t
node_get(const node_t *self, size_t index) {
    assert(index < self->ctor->arity);
    return array_get(self->value_array, index);
}

bool
node_is_adjacent(const node_t *self, const node_t *other) {
    if (!self->ctor) return false;
    if (!other->ctor) return false;

    for (size_t i = 0; i < self->ctor->arity; i++) {
        for (size_t j = 0; j < other->ctor->arity; j++) {
            value_t x = node_get(self, i);
            value_t y = node_get(other, j);
            if (is_wire(x) && is_wire(y)) {
                wire_t *u = as_wire(x);
                wire_t *v = as_wire(y);
                if (wire_opposite(u) == v && wire_opposite(v) == u) {
                    return true;
                }
            }
        }
    }

    return false;
}

hash_t *
build_node_adjacency_hash(node_allocator_t *node_allocator) {
    hash_t *adjacency_hash = hash_new();
    hash_set_destroy_fn(adjacency_hash, (destroy_fn_t *) array_destroy);

    array_t *node_array = allocated_node_array(node_allocator);
    size_t length = array_length(node_array);
    for (size_t i = 0; i < length; i++) {
        node_t *x = array_get(node_array, i);
        array_t *adjacency_array = array_auto();
        hash_set(adjacency_hash, x, adjacency_array);

        for (size_t j = 0; j < length; j++) {
            node_t *y = array_get(node_array, j);
            if (node_is_adjacent(x, y)) {
                array_push(adjacency_array, y);
            }
        }
    }

    array_destroy(&node_array);
    return adjacency_hash;
}

void
node_print(const node_t *self, file_t *file) {
    if (self->ctor) {
        char *id_string = uint_to_subscript(self->id);
        fprintf(file, "(%s%s)", self->ctor->name, id_string);
        free(id_string);
    } else {
        char *id_string = uint_to_subscript(self->id);
        fprintf(file, "(%s)", id_string);
        free(id_string);
    }
}
