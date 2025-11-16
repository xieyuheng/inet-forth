#include "index.h"

void
test_path(void) {
    test_start();

    {
        path_t *path = make_path("abc");
        assert(string_equal(path_string(path), "abc"));
        path_destroy(&path);
    }

    {
        path_t *path = make_path("");
        assert(string_equal(path_string(path), ""));
        path_destroy(&path);
    }

    {
        path_t *path = make_path("a/b/c");
        assert(string_equal(path_string(path), "a/b/c"));
        path_destroy(&path);
    }

    {
        path_t *path = make_path("aa/bb/cc");
        assert(string_equal(path_string(path), "aa/bb/cc"));
        path_destroy(&path);
    }

    {
        path_t *path = make_path("/aa/bb/cc");
        assert(path_is_absolute(path));
        assert(string_equal(path_string(path), "/aa/bb/cc"));
        path_destroy(&path);
    }

    {
        path_t *path = make_path("a/b/c");
        assert(string_equal(path_string(path), "a/b/c"));
        path_join(path, "d/e/f");
        assert(string_equal(path_string(path), "a/b/c/d/e/f"));
        path_destroy(&path);
    }

    {
        path_t *path = make_path("a/b/c");
        assert(string_equal(path_string(path), "a/b/c"));
        path_join(path, "/d/e/f");
        assert(string_equal(path_string(path), "a/b/c/d/e/f"));
        path_destroy(&path);
    }

    {
        path_t *path = make_path("/a/b/c");
        assert(string_equal(path_string(path), "/a/b/c"));
        path_join(path, "/d/e/f");
        assert(string_equal(path_string(path), "/a/b/c/d/e/f"));
        path_destroy(&path);
    }

    {
        path_t *path = make_path("/a/b/c");
        assert(string_equal(path_string(path), "/a/b/c"));
        path_join(path, "../../d/e/f");
        assert(string_equal(path_string(path), "/a/d/e/f"));
        path_destroy(&path);
    }

    {
        path_t *x = make_path("/a/b/c");
        path_t *y = make_path("/a/b/c/d/../d/../");
        assert(path_equal(x, y));
        path_destroy(&x);
        path_destroy(&y);
    }

    {
        path_t *x = make_path("../a/..");
        path_t *y = make_path("..");
        assert(path_equal(x, y));
        path_destroy(&x);
        path_destroy(&y);
    }

    {
        path_t *x = make_path("../a/../../b/..");
        path_t *y = make_path("../..");
        assert(path_equal(x, y));
        path_destroy(&x);
        path_destroy(&y);
    }

    {
        path_t *x = make_path("/a/b/c");
        path_t *y = make_path("////a/////b/////c/////");
        assert(path_equal(x, y));
        path_destroy(&x);
        path_destroy(&y);
    }

    {
        path_t *x = make_path("/a/b/c");
        path_t *y = make_path("/./././a/./././////b/././././c/././");
        assert(path_equal(x, y));
        path_destroy(&x);
        path_destroy(&y);
    }

    {
        path_t *x = make_path("/a/b/c");
        path_t *y = make_path("/a/b/d");
        path_t *z = path_relative(x, y);
        path_t *w = make_path("../d");
        assert(path_equal(z, w));
        path_destroy(&x);
        path_destroy(&y);
        path_destroy(&z);
        path_destroy(&w);
    }

    {
        path_t *x = make_path("/a/b/c");
        path_t *y = make_path("/a/b");
        path_t *z = path_relative(x, y);
        path_t *w = make_path("..");
        assert(path_equal(z, w));
        path_destroy(&x);
        path_destroy(&y);
        path_destroy(&z);
        path_destroy(&w);
    }

    {
        path_t *x = make_path("/a/b");
        path_t *y = make_path("/a/b/c");
        path_t *z = path_relative(x, y);
        path_t *w = make_path("c");
        assert(path_equal(z, w));
        path_destroy(&x);
        path_destroy(&y);
        path_destroy(&z);
        path_destroy(&w);
    }

    test_end();
}
