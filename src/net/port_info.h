#pragma once

struct port_info_t {
    char *name;
    bool is_principal;
};

port_info_t *port_info_new(char *name, bool is_principal);
void port_info_destroy(port_info_t **self_pointer);

port_info_t *port_info_from_name(char *name);
