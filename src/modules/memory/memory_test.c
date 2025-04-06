#include "index.h"

struct vec2_t {
    double x;
    double y;
};

typedef struct vec2_t vec2_t;

void
memory_test(void) {
    printf("<memory_test>\n");

    {
        void *pointer = allocate(10);
        assert(pointer_is_8_bytes_aligned(pointer));
        destroy(&pointer);
        destroy(&pointer);
    }

    {
        uint8_t *pointer = allocate(3);
        pointer[0] = 1;
        pointer[1] = 2;
        pointer[2] = 3;

        assert(pointer[0] == 1);
        assert(pointer[1] == 2);
        assert(pointer[2] == 3);

        pointer = reallocate(pointer, 3, 6);

        assert(pointer[0] == 1);
        assert(pointer[1] == 2);
        assert(pointer[2] == 3);
        assert(pointer[3] == 0);
        assert(pointer[4] == 0);
        assert(pointer[5] == 0);

        destroy((void **) &pointer);
    }

    {
        uint64_t *pointer = allocate_pointers(3);
        pointer[0] = 1;
        pointer[1] = 2;
        pointer[2] = 3;

        assert(pointer[0] == 1);
        assert(pointer[1] == 2);
        assert(pointer[2] == 3);

        pointer = reallocate_pointers(pointer, 3, 6);

        assert(pointer[0] == 1);
        assert(pointer[1] == 2);
        assert(pointer[2] == 3);
        assert(pointer[3] == 0);
        assert(pointer[4] == 0);
        assert(pointer[5] == 0);

        destroy((void **) &pointer);
    }

    {
        void *pointer = allocate_page_aligned(10);
        assert(pointer_is_8_bytes_aligned(pointer));
        assert(pointer_is_page_aligned(pointer));
        destroy(&pointer);
        destroy(&pointer);
    }

    {
        vec2_t *v = new(vec2_t);
        assert(sizeof *v == 16);
        assert(pointer_is_8_bytes_aligned(v));
        v->x = 0.1;
        v->y = 0.1;
        destroy((void **) &v);
        destroy((void **) &v);
    }

    {
        vec2_t *v = new_page_aligned(vec2_t);
        assert(sizeof *v == 16);
        assert(pointer_is_8_bytes_aligned(v));
        assert(pointer_is_page_aligned(v));
        v->x = 0.1;
        v->y = 0.1;
        destroy((void **) &v);
        destroy((void **) &v);
    }

    printf("</memory_test>\n");
}
