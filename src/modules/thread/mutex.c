#include "index.h"

mutex_t *
mutex_new(void) {
    mutex_t *self = new(mutex_t);
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
    int errno = pthread_mutex_init(self, &mutex_attr);
    assert(errno == 0);
    pthread_mutexattr_destroy(&mutex_attr);
    return self;
}

void
mutex_destroy(mutex_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    mutex_t *self = *self_pointer;
    int errno = pthread_mutex_destroy(self);
    assert(errno == 0);
    free(self);
    *self_pointer = NULL;
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
