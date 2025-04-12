#include "index.h"

void
image_test(void) {
    test_start();

    assert(image_hex_width_from_path("button10x10.chr") == 0x10);
    assert(image_hex_height_from_path("button10x10.chr") == 0x10);

    test_end();
}
