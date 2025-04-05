#include "index.h"

mutex_t *
mutex_new(void) {
    mutex_t *self = new(mutex_t);
    int errno = pthread_mutex_init(self, NULL);
    assert(errno == 0);
    return self;
}

void
mutex_destroy(mutex_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        mutex_t *self = *self_pointer;
        int errno = pthread_mutex_destroy(self);
        assert(errno == 0);
        free(self);
        *self_pointer = NULL;
    }
}

void
mutex_lock(mutex_t *self) {
    int errno = pthread_mutex_lock(self);
    assert(errno == 0);
}

void mutex_unlock(mutex_t *self) {
    int errno = pthread_mutex_unlock(self);
    assert(errno == 0);
}
