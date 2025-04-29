#include "index.h"

void
test_time(void) {
    test_start();

    who_printf("time_second(): %f\n", time_second());
    who_printf("time_second(): %f\n", time_second());
    who_printf("time_second(): %f\n", time_second());

    who_printf("time_nanosecond(): %lu\n", time_nanosecond());
    who_printf("time_nanosecond(): %lu\n", time_nanosecond());
    who_printf("time_nanosecond(): %lu\n", time_nanosecond());

    test_end();
}
