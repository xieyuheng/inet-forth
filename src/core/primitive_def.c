#include "index.h"

static primitive_def_t *
primitive_def_new(const char *name) {
    primitive_def_t *self = new(primitive_def_t);
    self->name = string_copy(name);
    return self;
}

primitive_def_t *
primitive_def_from_worker_fn(const char *name, primitive_fn_t *primitive_fn) {
    primitive_def_t *self = primitive_def_new(name);
    self->fn_kind = PRIMITIVE_VM_FN;
    self->primitive_fn = primitive_fn;
    return self;
}

primitive_def_t *
primitive_def_from_fn_0(const char *name, primitive_fn_0_t *primitive_fn_0) {
    primitive_def_t *self = primitive_def_new(name);
    self->fn_kind = PRIMITIVE_0_FN;
    self->primitive_fn_0 = primitive_fn_0;
    return self;
}

primitive_def_t *
primitive_def_from_fn_1(const char *name, primitive_fn_1_t *primitive_fn_1) {
    primitive_def_t *self = primitive_def_new(name);
    self->fn_kind = PRIMITIVE_1_FN;
    self->primitive_fn_1 = primitive_fn_1;
    return self;
}

primitive_def_t *
primitive_def_from_fn_2(const char *name, primitive_fn_2_t *primitive_fn_2) {
    primitive_def_t *self = primitive_def_new(name);
    self->fn_kind = PRIMITIVE_2_FN;
    self->primitive_fn_2 = primitive_fn_2;
    return self;
}

primitive_def_t *
primitive_def_from_fn_3(const char *name, primitive_fn_3_t *primitive_fn_3) {
    primitive_def_t *self = primitive_def_new(name);
    self->fn_kind = PRIMITIVE_3_FN;
    self->primitive_fn_3 = primitive_fn_3;
    return self;
}

primitive_def_t *
primitive_def_from_fn_4(const char *name, primitive_fn_4_t *primitive_fn_4) {
    primitive_def_t *self = primitive_def_new(name);
    self->fn_kind = PRIMITIVE_4_FN;
    self->primitive_fn_4 = primitive_fn_4;
    return self;
}

void
primitive_def_destroy(primitive_def_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        primitive_def_t *self = *self_pointer;
        string_destroy(&self->name);
        free(self);
        *self_pointer = NULL;
    }
}
