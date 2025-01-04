#pragma once

struct xobject_spec_t {
    const char *name;
};

struct xobject_t {
    const xobject_spec_t *spec;
};
