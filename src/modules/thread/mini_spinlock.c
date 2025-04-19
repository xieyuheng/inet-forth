#include "index.h"

struct mini_spinlock_t {
    atomic_bool atomic_is_locked;
};
