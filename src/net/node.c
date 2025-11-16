#include "index.h"

node_t *
node_new(void) {
    node_t *self = new(node_t);
    self->mutex = make_mutex();
    self->values = allocate_pointers(NODE_MAX_ARITY);
    return self;
}

void
node_destroy(node_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_t *self = *self_pointer;
    mutex_destroy(&self->mutex);
    free(self->values);
    free(self);
    *self_pointer = NULL;
}

void
node_clean(node_t *self) {
    self->ctor = NULL;
    self->locked_by_worker = NULL;
    self->is_allocated = false;
    memset(self->values, 0, NODE_MAX_ARITY * sizeof(void *));
}

void
node_put_value(node_t *self, size_t index, value_t value) {
    assert(self);
    assert(index < NODE_MAX_ARITY);
    self->values[index] = value;
}

value_t
node_get_value(const node_t *self, size_t index) {
    assert(self);
    assert(self->ctor);
    assert(index < NODE_MAX_ARITY);
    return self->values[index];
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
