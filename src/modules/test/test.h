#pragma once

#define test_start(...) printf("[%s] start\n", __func__)
#define test_end(...) printf("")
#define test_printf(...) printf("[%s] ", __func__); printf(__VA_ARGS__)
