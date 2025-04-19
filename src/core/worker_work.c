#include "index.h"

void
worker_work(worker_t *worker) {
    if (single_threaded_flag) {
        worker_work_sequentially(worker);
    } else {
        worker_work_parallelly(worker);
    }
}
