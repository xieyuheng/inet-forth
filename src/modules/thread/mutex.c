#include "index.h"

mutex_t *
mutex_new(void) {
    mutex_t *self = new(mutex_t);
    int ok = pthread_mutex_init(self, NULL);
    assert(ok == 0);
    return self;
}
