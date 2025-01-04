#pragma once

typedef void (print_fn_t)(const void *value, file_t *file);

struct xobject_spec_t {
    const char *name;
    print_fn_t *print_fn;
};

struct xobject_t {
    const xobject_spec_t *spec;
};

bool is_xobject(value_t value);
