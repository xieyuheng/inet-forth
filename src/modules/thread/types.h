#pragma once

typedef void *(thread_fn_t)(void *arg);

typedef pthread_t tid_t;
typedef pthread_mutex_t mutex_t;
typedef pthread_spinlock_t spinlock_t;
typedef struct fast_spinlock_t fast_spinlock_t;
