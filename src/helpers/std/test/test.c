#include "index.h"

double
test_time_millisecond(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    double millisecond = ts.tv_sec * 1000 + ts.tv_nsec * 1e-6;
    return millisecond;
}

double
test_time_passed_millisecond(double start_millisecond) {
    double end_millisecond = test_time_millisecond();
    return end_millisecond - start_millisecond;
}
