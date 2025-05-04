#include "index.h"

spinlock_t *
spinlock_new(void) {
    spinlock_t *self = new(spinlock_t);
    int errno = pthread_spin_init(self, PTHREAD_PROCESS_PRIVATE);
    assert(errno == 0);
    return self;
}

void
spinlock_destroy(spinlock_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    spinlock_t *self = *self_pointer;
    int errno = pthread_spin_destroy(self);
    assert(errno == 0);
    // We need to cast pointer to volatile data to normal pointer.
    free((void *) self);
    *self_pointer = NULL;
}

void
spinlock_lock(spinlock_t *self) {
    int errno = pthread_spin_lock(self);
    assert(errno == 0);
}

bool
spinlock_try_lock(spinlock_t *self) {
    int errno = pthread_spin_trylock(self);
    return errno == 0;
}

void spinlock_unlock(spinlock_t *self) {
    int errno = pthread_spin_unlock(self);
    assert(errno == 0);
}
