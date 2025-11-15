#include "index.h"

tid_t
thread_start(thread_fn_t *thread_fn, void *arg) {
    tid_t tid;
    int ok = pthread_create(&tid, NULL, thread_fn, arg);
    assert(ok == 0);
    return tid;
}

tid_t
thread_tid(void) {
    return pthread_self();
}

void *
thread_wait(tid_t tid) {
    void *result;
    int ok = pthread_join(tid, &result);
    assert(ok == 0);
    return result;
}

bool
tid_equal(tid_t T1, tid_t T2) {
    return pthread_equal(T1, T2);
}

void
tid_print(tid_t tid) {
    printf("%lu", (uint64_t) tid);
}
