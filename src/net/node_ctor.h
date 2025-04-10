#include "index.h"

extern object_spec_t node_ctor_object_spec;

struct node_ctor_t {
    object_spec_t *spec;
    char *name;
    size_t input_arity;
    size_t output_arity;
    size_t arity;
    port_info_t **port_infos;
    array_t *rule_array;
};

node_ctor_t *node_ctor_new(
    const char *name,
    size_t input_arity,
    size_t output_arity);
void node_ctor_destroy(node_ctor_t **self_pointer);

bool is_node_ctor(value_t value);
node_ctor_t *as_node_ctor(value_t value);

size_t node_ctor_find_port_index(const node_ctor_t *node_ctor, const char *port_name);

void node_ctor_print(const node_ctor_t *self, file_t *file);
