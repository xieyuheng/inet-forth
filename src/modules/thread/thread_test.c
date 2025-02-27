#include "index.h"

static void *
thread_fn(char *message) {
    printf("[thread_fn] %s\n", message);
    return (void *) strlen(message);
}

void
thread_test(void) {
    printf("<thread_test>\n");

    char *message = string_copy("hello thread");
    thread_id_t thread_id = thread_start((thread_fn_t *) thread_fn, message);

    printf("[thread_test]thread created: %lu\n", thread_id);
    size_t length = (size_t) thread_wait(thread_id);
    printf("[thread_test] thread returned: %lu\n", length);

    printf("</thread_test>\n");
}
