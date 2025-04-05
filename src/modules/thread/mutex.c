#include "index.h"

mutex_t *
mutex_new(void) {
    mutex_t *self = new(mutex_t);
    int err = pthread_mutex_init(self, NULL);
    assert(err == 0);
    return self;
}

void
mutex_destroy(mutex_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        mutex_t *self = *self_pointer;
        int err = pthread_mutex_destroy(self);
        assert(err == 0);
        free(self);
        *self_pointer = NULL;
    }
}
