#include "index.h"

void
glyph_test(void) {
    test_start();

    glyph_t *glyph = glyph_decode_hex("0041:0000000018242442427E424242420000");

    glyph_print_ascii_art(glyph, '-', 'A');
    printf("\n");

    glyph_set(glyph, 0x0, 0x0, true);
    glyph_set(glyph, 0x7, 0x0, true);
    glyph_set(glyph, 0x0, 0xf, true);
    glyph_set(glyph, 0x7, 0xf, true);

    glyph_print_ascii_art(glyph, '-', 'A');
    printf("\n");

    glyph_set(glyph, 0x0, 0x0, false);
    glyph_set(glyph, 0x7, 0x0, false);
    glyph_set(glyph, 0x0, 0xf, false);
    glyph_set(glyph, 0x7, 0xf, false);

    glyph_print_ascii_art(glyph, '-', 'A');
    printf("\n");

    glyph_destroy(&glyph);

    test_end();
}
