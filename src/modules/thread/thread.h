#pragma once

thread_id_t thread_start(thread_fn_t *thread_fn, void *arg);

void *thread_wait(thread_id_t thread_id);
