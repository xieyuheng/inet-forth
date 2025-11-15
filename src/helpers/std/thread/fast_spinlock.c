#include "index.h"

struct fast_spinlock_t {
    atomic_bool atomic_is_locked;
};

fast_spinlock_t *
fast_spinlock_new(void) {
    fast_spinlock_t *self = new(fast_spinlock_t);
    atomic_init(&self->atomic_is_locked, false);
    return self;
}

void
fast_spinlock_destroy(fast_spinlock_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    fast_spinlock_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}

void
fast_spinlock_lock(fast_spinlock_t *self) {
    while (atomic_load_explicit(
               &self->atomic_is_locked,
               memory_order_relaxed) ||
           atomic_exchange_explicit(
               &self->atomic_is_locked,
               true,
               memory_order_acquire))
    {
        time_sleep_nanosecond(1);
    }
}

bool
fast_spinlock_try_lock(fast_spinlock_t *self) {
    return !(atomic_load_explicit(
                 &self->atomic_is_locked,
                 memory_order_relaxed) ||
             atomic_exchange_explicit(
                 &self->atomic_is_locked,
                 true,
                 memory_order_acquire));
}

void
fast_spinlock_unlock(fast_spinlock_t *self) {
    atomic_store_explicit(
        &self->atomic_is_locked,
        false,
        memory_order_release);
}
