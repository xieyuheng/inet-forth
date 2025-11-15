#include "index.h"

void
test_set(void) {
    test_start();

    {
        set_t *set = set_new();
        assert(set_length(set) == 0);

        assert(set_add(set, (void *) 1));
        assert(set_add(set, (void *) 2));
        assert(set_add(set, (void *) 3));

        assert(set_length(set) == 3);

        assert(!set_add(set, (void *) 1));
        assert(!set_add(set, (void *) 2));
        assert(!set_add(set, (void *) 3));

        assert(set_length(set) == 3);

        assert(set_has(set, (void *) 1));
        assert(set_has(set, (void *) 2));
        assert(set_has(set, (void *) 3));

        assert(!set_has(set, (void *) 0));
        assert(!set_has(set, (void *) 4));

        assert(set_delete(set, (void *) 2));
        assert(set_delete(set, (void *) 3));

        assert(!set_delete(set, (void *) 2));
        assert(!set_delete(set, (void *) 3));

        assert(set_length(set) == 1);

        assert(set_has(set, (void *) 1));
        assert(!set_has(set, (void *) 2));
        assert(!set_has(set, (void *) 3));

        set_destroy(&set);
    }

    test_end();
}
