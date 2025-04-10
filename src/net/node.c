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

void
node_print_name(const node_t *self, file_t *file) {
    if (self->ctor) {
        char *id_string = uint_to_subscript(self->id);
        fprintf(file, "%s%s", self->ctor->name, id_string);
        free(id_string);
    } else {
        char *id_string = uint_to_subscript(self->id);
        fprintf(file, "%s", id_string);
        free(id_string);
    }
}

void
node_print(const node_t *self, file_t *file) {
    fprintf(file, "(");
    node_print_name(self, file);
    fprintf(file, ")");
}

void
node_print_connected_net(node_t *self, hash_t *node_adjacency_hash, file_t *file) {
    assert(self);
    fprintf(file, "<net>\n");

    fprintf(file, ":root ");
    node_print(self, file);
    fprintf(file, "\n");

    connected_node_iter_t *node_iter = connected_node_iter_new(self, node_adjacency_hash);
    node_t *node = connected_node_iter_first(node_iter);
    while (node) {
        fprintf(file, "(");
        node_print_name(node, file);
        if (!node->ctor) {
            fprintf(file, ")");
            fprintf(file, "\n");
            continue;
        }

        fprintf(file, "\n");
        for (size_t i = 0; i < node->ctor->arity; i++) {
            port_info_t *port_info = node->ctor->port_infos[i];
            if (port_info->is_principal)
                fprintf(file, " :%s! ", port_info->name);
            else
                fprintf(file, " :%s ", port_info->name);

            value_t value = node_get(node, i);
            if (value) {
                if (is_wire(value)) {
                    wire_print(as_wire(value), file);
                } else {
                    value_print(value, file);
                }
            }

            if (i < node->ctor->arity - 1)
                fprintf(file, "\n");
        }
        fprintf(file, ")");
        fprintf(file, "\n");

        node = connected_node_iter_next(node_iter);
    }

    connected_node_iter_destroy(&node_iter);
    fprintf(file, "</net>\n");
}
