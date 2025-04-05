#include "index.h"

static void *
thread_fn(void *arg) {
    char *message = arg;
    printf("[thread_fn] %s\n", message);
    return (void *) strlen(message);
}

void
thread_test_start(void) {
    printf("<thread_test_start>\n");

    char *message = string_copy("hello thread");
    thread_id_t thread_id = thread_start(thread_fn, message);

    printf("thread created: %lu\n", (uint64_t) thread_id);
    size_t length = (size_t) thread_wait(thread_id);
    printf("thread returned: %lu\n", length);

    printf("</thread_test_start>\n");
}
