#include "index.h"

object_spec_t primitive_object_spec = {
    .name = "primitive",
    .print_fn = (print_fn_t *) primitive_print,
};

static primitive_t *
primitive_new(const char *name) {
    primitive_t *self = new(primitive_t);
    self->spec = &primitive_object_spec;
    self->name = string_copy(name);
    return self;
}

primitive_t *
primitive_from_fn(const char *name, primitive_fn_t *primitive_fn) {
    primitive_t *self = primitive_new(name);
    self->fn_kind = PRIMITIVE_FN;
    self->primitive_fn = primitive_fn;
    return self;
}

primitive_t *
primitive_from_fn_0(const char *name, primitive_fn_0_t *primitive_fn_0) {
    primitive_t *self = primitive_new(name);
    self->fn_kind = PRIMITIVE_FN_0;
    self->primitive_fn_0 = primitive_fn_0;
    return self;
}

primitive_t *
primitive_from_fn_1(const char *name, primitive_fn_1_t *primitive_fn_1) {
    primitive_t *self = primitive_new(name);
    self->fn_kind = PRIMITIVE_FN_1;
    self->primitive_fn_1 = primitive_fn_1;
    return self;
}

primitive_t *
primitive_from_fn_2(const char *name, primitive_fn_2_t *primitive_fn_2) {
    primitive_t *self = primitive_new(name);
    self->fn_kind = PRIMITIVE_FN_2;
    self->primitive_fn_2 = primitive_fn_2;
    return self;
}

primitive_t *
primitive_from_fn_3(const char *name, primitive_fn_3_t *primitive_fn_3) {
    primitive_t *self = primitive_new(name);
    self->fn_kind = PRIMITIVE_FN_3;
    self->primitive_fn_3 = primitive_fn_3;
    return self;
}

primitive_t *
primitive_from_fn_4(const char *name, primitive_fn_4_t *primitive_fn_4) {
    primitive_t *self = primitive_new(name);
    self->fn_kind = PRIMITIVE_FN_4;
    self->primitive_fn_4 = primitive_fn_4;
    return self;
}

void
primitive_destroy(primitive_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    primitive_t *self = *self_pointer;
    string_destroy(&self->name);
    free(self);
    *self_pointer = NULL;
}

bool
is_primitive(value_t value) {
    if (!is_xobject(value)) return false;
    return as_object(value)->spec == &primitive_object_spec;
}

primitive_t *
as_primitive(value_t value) {
    assert(is_primitive(value));
    return (primitive_t *) value;
}

void
primitive_print(primitive_t *self, file_t *file) {
    fprintf(file, "%s", self->name);
}
