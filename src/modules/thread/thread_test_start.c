#include "index.h"

static void *
example_thread_fn(char *message) {
    printf("[example_thread_fn] %s\n", message);
    return (void *) strlen(message);
}

void
thread_test_start(void) {
    printf("<thread_test_start>\n");

    char *message = string_copy("hello thread");
    thread_fn_t *thread_fn = (thread_fn_t *) example_thread_fn;
    thread_id_t thread_id = thread_start(thread_fn, message);

    printf("thread created: %lu\n", (uint64_t) thread_id);
    size_t length = (size_t) thread_wait(thread_id);
    printf("thread returned: %lu\n", length);

    printf("</thread_test_start>\n");
}
