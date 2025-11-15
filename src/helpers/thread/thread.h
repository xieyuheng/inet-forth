#pragma once

tid_t thread_start(thread_fn_t *thread_fn, void *arg);
tid_t thread_tid(void);
void *thread_wait(tid_t tid);

bool tid_equal(tid_t T1, tid_t T2);
void tid_print(tid_t tid);
