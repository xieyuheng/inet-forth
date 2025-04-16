#include "index.h"

node_t *
node_new(void) {
    node_t *self = new(node_t);
#if DEBUG_NODE_MUTEX
    self->mutex = mutex_new();
#endif
    self->value_array = array_new_auto();
    return self;
}

void
node_destroy(node_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_t *self = *self_pointer;
    array_destroy(&self->value_array);
#if DEBUG_NODE_MUTEX
    // mutex_destroy(&self->mutex);
#endif
    free(self);
    *self_pointer = NULL;
}

void
node_set_value(node_t *self, size_t index, value_t value) {
    assert(self);
    if (!self->ctor) {
        file_lock(stdout);
        test_printf("expect node to have ctor\n");
        test_printf("index: %ld\n", index);
        test_printf("node: "); node_print(self, stdout); printf("\n");
        test_printf("value: "); value_print(value, stdout); printf("\n");
        test_printf("\n");
        file_unlock(stdout);
        assert(self->ctor);
    }

    array_set(self->value_array, index, value);
}

value_t
node_get_value(const node_t *self, size_t index) {
    assert(self->ctor);
    assert(index < self->ctor->arity);
    return array_get(self->value_array, index);
}

port_info_t *
node_get_port_info(const node_t *self, size_t index) {
    assert(self->ctor);
    assert(index < self->ctor->arity);
    return self->ctor->port_infos[index];
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
node_print_connected(node_t *self, hash_t *node_adjacency_hash, file_t *file) {
    fprintf(file, "<net>\n");

    fprintf(file, ":root ");
    node_print(self, file);
    fprintf(file, "\n");

    fprintf(file, "\n");

    array_t *node_array = connected_node_array(self, node_adjacency_hash);
    for (size_t i = 0; i < array_length(node_array); i++) {
        node_t *node = array_get(node_array, i);
        array_t *node_adjacency_array = hash_get(node_adjacency_hash, node);
        assert(node_adjacency_array);
        node_adjacency_array_print(node_adjacency_array, file);
        if (i != array_length(node_array) - 1)
            fprintf(file, "\n");
    }

    array_destroy(&node_array);
    fprintf(file, "</net>\n");
}

bool
node_has_wire(node_t *node, wire_t *wire) {
    for (size_t i = 0; i < node->ctor->arity; i++) {
        if (is_fuzed(node_get_value(node, i), wire))
            return true;
    }

    return false;
}
