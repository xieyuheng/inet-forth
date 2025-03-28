#include "index.h"

struct wire_iter_t {
    wire_t *root;
    list_t *occurred_wire_list;
    list_t *remaining_node_list;
    node_t *node;
    size_t index;
};

wire_iter_t *
wire_iter_new(wire_t *root) {
    wire_iter_t *self = new(wire_iter_t);
    self->root = root;
    self->occurred_wire_list = list_new();
    self->remaining_node_list = list_new();
    return self;
}

void
wire_iter_destroy(wire_iter_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        wire_iter_t *self = *self_pointer;
        list_destroy(&self->occurred_wire_list);
        list_destroy(&self->remaining_node_list);
        free(self);
        *self_pointer = NULL;
    }
}

wire_t *
wire_iter_first(wire_iter_t *self) {
    list_push(self->occurred_wire_list, self->root);

    wire_t *opposite = wire_opposite(self->root);
    if (opposite) {
        list_push(self->occurred_wire_list,
                  opposite);
    }

    if (opposite &&
        opposite->node)
    {
        self->node = opposite->node;
        self->index = 0;
    }

    return self->root;
}

wire_t *
wire_iter_next(wire_iter_t *self) {
    if (!self->node) return NULL;

    while (self->index < self->node->ctor->arity) {
        size_t i = self->index++;
        value_t value = node_get(self->node, i);
        if (!is_wire(value)) continue;

        wire_t *wire = as_wire(value);

        if (list_has(self->occurred_wire_list, wire))
            continue;

        list_push(self->occurred_wire_list, wire);

        wire_t *opposite = wire_opposite(wire);
        if (opposite) {
            if (list_has(self->occurred_wire_list, opposite))
                continue;

            list_push(self->occurred_wire_list, opposite);

            if (opposite->node &&
                !list_has(self->remaining_node_list, opposite->node))
            {
                list_push(self->remaining_node_list, opposite->node);
            }
        }

        return wire;
    }

    self->node = list_pop(self->remaining_node_list);
    self->index = 0;
    return wire_iter_next(self);
}
