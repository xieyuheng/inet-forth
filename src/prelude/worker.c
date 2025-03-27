#include "index.h"

void
x_print_worker(worker_t *worker) {
    worker_print(worker, stdout);
    fprintf(stdout, "\n");
}

void
x_print_value_stack(worker_t *worker) {
    worker_print_value_stack(worker, stdout);
}

void
x_print_return_stack(worker_t *worker) {
    worker_print_return_stack(worker, stdout);
}
