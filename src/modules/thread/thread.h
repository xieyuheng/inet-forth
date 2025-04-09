#pragma once

tid_t thread_start(thread_fn_t *thread_fn, void *arg);
tid_t thread_tid(void);
void *thread_wait(tid_t tid);
