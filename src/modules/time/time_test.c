#include "index.h"

void
time_test(void) {
    test_start();

    test_printf("time_second(): %f\n", time_second());
    test_printf("time_second(): %f\n", time_second());
    test_printf("time_second(): %f\n", time_second());

    test_end();
}
