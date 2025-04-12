#include "index.h"

void
time_test(void) {
    test_start();

    printf("time_second(): %f\n", time_second());
    printf("time_second(): %f\n", time_second());
    printf("time_second(): %f\n", time_second());

    test_end();
}
