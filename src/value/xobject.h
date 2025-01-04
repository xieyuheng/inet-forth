#pragma once

typedef void (print_fn_t)(const void *value, file_t *file);

struct object_spec_t {
    const char *name;
    print_fn_t *print_fn;
};

struct object_t {
    const object_spec_t *spec;
};

value_t xobject(object_t *target);
bool is_xobject(value_t value);
object_t *to_object(value_t value);
