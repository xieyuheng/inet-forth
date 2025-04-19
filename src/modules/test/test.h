#pragma once

double test_time_millisecond(void);
double test_time_passed_millisecond(double start_millisecond);

#define test_start(...) \
    double test_start_millisecond = test_time_millisecond();    \
    printf("[%s] start\n", __func__)

#define test_end(...) printf("[%s] end: %.3f ms\n", __func__, test_time_passed_millisecond(test_start_millisecond))

#define who_printf(...) printf("[%s] ", __func__); printf(__VA_ARGS__)
