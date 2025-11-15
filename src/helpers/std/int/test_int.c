#include "index.h"

void
test_int(void) {
    test_start();

    assert(uint_max(1, 2) == 2);
    assert(uint_min(1, 2) == 1);

    assert(string_equal(uint_to_string(123), "123"));
    assert(string_equal(uint_to_subscript(123), "₁₂₃"));
    assert(string_equal(uint_to_superscript(123), "¹²³"));

    assert(uint_decimal_length(1) == 1);
    assert(uint_decimal_length(12) == 2);
    assert(uint_decimal_length(123) == 3);
    assert(uint_decimal_length(1234) == 4);

    {
        assert(int_relu(1) == 1);
        assert(int_relu(-1) == 0);

        int x = -1;
        assert(int_relu(x) == 0);
    }

    test_end();
}
