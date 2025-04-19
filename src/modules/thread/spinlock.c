#include "index.h"

spinlock_t *
spinlock_new(void) {
    spinlock_t *self = new(spinlock_t);
    int errno = pthread_spin_init(self, PTHREAD_PROCESS_PRIVATE);
    assert(errno == 0);
    return self;
}

// void spinlock_destroy(spinlock_t **self_pointer);

// void spinlock_lock(spinlock_t *self);
// bool spinlock_try_lock(spinlock_t *self);
// void spinlock_unlock(spinlock_t *self);
