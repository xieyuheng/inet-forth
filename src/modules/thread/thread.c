#include "index.h"

tid_t
thread_start(thread_fn_t *thread_fn, void *arg) {
    tid_t tid;
    int ok = pthread_create(&tid, NULL, thread_fn, arg);
    assert(ok == 0);
    return tid;
}

void *
thread_wait(tid_t tid) {
    void *result;
    int ok = pthread_join(tid, &result);
    assert(ok == 0);
    return result;
}
