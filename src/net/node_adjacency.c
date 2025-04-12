#include "index.h"

node_adjacency_t *
node_adjacency_new(node_t *start_node, node_t *end_node) {
    assert(node_is_adjacent(start_node, end_node));

    node_adjacency_t *self = new(node_adjacency_t);
    self->start_node = start_node;
    self->end_node = end_node;

    for (size_t i = 0; i < start_node->ctor->arity; i++) {
        for (size_t j = 0; j < end_node->ctor->arity; j++) {
            value_t x = node_get_value(start_node, i);
            value_t y = node_get_value(end_node, j);
            if (is_fuzed(x, y)) {
                self->start_port_index = i;
                self->end_port_index = j;
            }
        }
    }

    return self;
}

void
node_adjacency_destroy(node_adjacency_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_adjacency_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}

hash_t *
build_node_adjacency_hash(node_allocator_t *node_allocator) {
    hash_t *node_adjacency_hash = hash_new();
    hash_set_destroy_fn(node_adjacency_hash, (destroy_fn_t *) array_destroy);

    array_t *node_array = allocated_node_array(node_allocator);
    size_t length = array_length(node_array);
    for (size_t i = 0; i < length; i++) {
        node_t *x = array_get(node_array, i);
        array_t *node_adjacency_array =
            array_auto_with((destroy_fn_t *) node_adjacency_destroy);
        hash_set(node_adjacency_hash, x, node_adjacency_array);

        for (size_t j = 0; j < length; j++) {
            node_t *y = array_get(node_array, j);
            if (node_is_adjacent(x, y)) {
                array_push(node_adjacency_array, node_adjacency_new(x, y));
            }
        }
    }

    array_destroy(&node_array);
    return node_adjacency_hash;
}

void
node_adjacency_print(const node_adjacency_t *self, file_t *file) {
    port_info_t *start_port_info = node_get_port_info(self->start_node, self->start_port_index);
    port_info_t *end_port_info = node_get_port_info(self->end_node, self->end_port_index);

    node_print(self->start_node, file);

    if (start_port_info->is_principal)
        fprintf(file, "-%s!-<", start_port_info->name);
    else
        fprintf(file, "-%s-<", start_port_info->name);

    if (start_port_info->is_principal)
        fprintf(file, ">-!%s-", end_port_info->name);
    else
        fprintf(file, ">-%s-", end_port_info->name);

    node_print(self->end_node, file);
}

void
node_adjacency_array_print(array_t *node_adjacency_array, file_t *file) {
    assert(node_adjacency_array);
    size_t length = array_length(node_adjacency_array);
    for (size_t i = 0; i < length; i++) {
        node_adjacency_t *node_adjacency = array_get(node_adjacency_array, i);
        node_adjacency_print(node_adjacency, file);
        fprintf(file, "\n");
    }
}
