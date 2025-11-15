#include "index.h"

static double add1(double x) { return x + 1; }

void
test_vec2(void) {
    test_start();

    assert(
        vec2_equal(
            vec2_map(vec2_zeros(), add1),
            vec2_ones()));

    test_end();
}
