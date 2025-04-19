#include "index.h"

struct mini_spinlock_t {
    atomic_bool atomic_is_locked;
};

mini_spinlock_t *
mini_spinlock_new(void) {
    mini_spinlock_t *self = new(mini_spinlock_t);
    atomic_init(&self->atomic_is_locked, false);
    return self;
}

void
mini_spinlock_destroy(mini_spinlock_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    mini_spinlock_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}
