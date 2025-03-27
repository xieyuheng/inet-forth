#include "index.h"

void
execute_all(worker_t *worker) {
    while (!list_is_empty(worker->token_list)) {
        execute_one(worker);
    }
}
