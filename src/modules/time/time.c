#include "index.h"

double
time_second(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    double second = ts.tv_sec + ts.tv_nsec * 1e-9;
    return second;
}
