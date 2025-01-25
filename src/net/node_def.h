#pragma once

struct node_def_t {
    char *name;
    size_t input_arity;
    size_t output_arity;
    size_t arity;
    port_info_t **port_infos;
};

node_def_t *node_def_new(
    const char *name,
    size_t input_arity,
    size_t output_arity);
void node_def_destroy(node_def_t **self_pointer);

size_t node_def_find_port_index(const node_def_t *node_def, const char *port_name);
