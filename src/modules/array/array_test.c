#include "index.h"

void
array_test(void) {
    test_start();

    array_t *array = array_new_with(
        100, (destroy_fn_t *) string_destroy);

    assert(array);
    assert(array_size(array) == 100);
    assert(array_length(array) == 0);
    assert(array_is_empty(array));

    char *cheese = string_copy("boursin");
    char *bread = string_copy("baguette");
    char *wine = string_copy("bordeaux");

    array_push(array, cheese);
    assert(array_length(array) == 1);
    assert(!array_is_empty(array));

    array_push(array, bread);
    assert(array_length(array) == 2);
    assert(!array_is_empty(array));

    array_push(array, wine);
    assert(array_length(array) == 3);
    assert(!array_is_empty(array));

    assert(array_pick(array, 0) == wine);
    assert(array_pick(array, 1) == bread);
    assert(array_pick(array, 2) == cheese);

    assert(array_get(array, 2) == wine);
    assert(array_get(array, 1) == bread);
    assert(array_get(array, 0) == cheese);

    assert(array_pop(array) == wine);
    assert(array_pop(array) == bread);
    assert(array_pop(array) == cheese);

    assert(array_length(array) == 0);
    assert(array_is_empty(array));

    array_push(array, cheese);
    array_push(array, bread);
    array_push(array, wine);
    assert(array_length(array) == 3);

    array_purge(array);
    assert(array_length(array) == 0);
    assert(array_is_empty(array));

    array_destroy(&array);
    assert(array == NULL);

    {
        char *cheese = string_copy("boursin");
        char *bread = string_copy("baguette");
        char *wine = string_copy("bordeaux");

        array_t *array = array_new_with(
            3, (destroy_fn_t *) string_destroy);

        assert(!array_is_full(array));

        array_push(array, cheese);  assert(!array_is_full(array));
        array_push(array, bread);   assert(!array_is_full(array));
        array_push(array, wine);    assert(array_is_full(array));

        array_destroy(&array);
        assert(array == NULL);
    }

    {
        // cast atom value to void *.

        array_t *array = array_new(3);

        array_push(array, (void *) 1);
        array_push(array, (void *) 0);
        array_push(array, (void *) -1);

        assert(((int64_t) array_pop(array)) == -1);
        assert(((int64_t) array_pop(array)) == 0);
        assert(((int64_t) array_pop(array)) == 1);

        array_destroy(&array);
        assert(array == NULL);
    }

    {
        // array_resize

        array_t *array = array_new(3);

        array_push(array, (void *) 1);
        array_push(array, (void *) 2);
        array_push(array, (void *) 3);

        assert(((uint64_t) array_get(array, 0)) == 1);
        assert(((uint64_t) array_get(array, 1)) == 2);
        assert(((uint64_t) array_get(array, 2)) == 3);

        assert(array_is_full(array));

        array_resize(array, 6);

        assert(((uint64_t) array_get(array, 0)) == 1);
        assert(((uint64_t) array_get(array, 1)) == 2);
        assert(((uint64_t) array_get(array, 2)) == 3);

        assert(!array_is_full(array));

        array_push(array, (void *) 4);
        array_push(array, (void *) 5);
        array_push(array, (void *) 6);

        assert(((uint64_t) array_get(array, 3)) == 4);
        assert(((uint64_t) array_get(array, 4)) == 5);
        assert(((uint64_t) array_get(array, 5)) == 6);

        assert(array_is_full(array));

        array_destroy(&array);
        assert(array == NULL);
    }

    {
        // array_push + auto grow

        array_t *array = array_new(3);

        array_push(array, (void *) 1);
        array_push(array, (void *) 2);
        array_push(array, (void *) 3);

        assert(array_is_full(array));

        array_push(array, (void *) 4);
        assert(!array_is_full(array));
        assert(array_size(array) == 6);

        array_push(array, (void *) 5);
        array_push(array, (void *) 6);

        assert(array_is_full(array));

        array_destroy(&array);
        assert(array == NULL);
    }

    {
        // array_set + auto grow

        array_t *array = array_new(3);

        array_set(array, 4, (void *) 1);
        assert(array_length(array) == 5);

        assert(array_get(array, 0) == NULL);
        assert(array_get(array, 1) == NULL);
        assert(array_get(array, 2) == NULL);
        assert(array_get(array, 3) == NULL);
        assert(array_get(array, 4) == (void *) 1);
        assert(array_get(array, 5) == NULL);

        array_destroy(&array);
        assert(array == NULL);
    }

    {
        // array_set + auto grow -- again

        array_t *array = array_new_auto();

        array_set(array, 0, (void *) 1);
        assert(array_length(array) == 1);

        assert(array_get(array, 0) == (void *) 1);
        assert(array_get(array, 1) == NULL);

        array_destroy(&array);
        assert(array == NULL);
    }

    test_end();
}
