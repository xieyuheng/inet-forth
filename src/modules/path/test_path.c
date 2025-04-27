#include "index.h"

void
test_path(void) {
    test_start();

    {
        path_t *path = path_new("abc");
        assert(string_equal(path_string(path), "abc"));
        path_destroy(&path);
    }

    {
        path_t *path = path_new("");
        assert(string_equal(path_string(path), ""));
        path_destroy(&path);
    }

    {
        path_t *path = path_new("a/b/c");
        assert(string_equal(path_string(path), "a/b/c"));
        path_destroy(&path);
    }

    {
        path_t *path = path_new("aa/bb/cc");
        assert(string_equal(path_string(path), "aa/bb/cc"));
        path_destroy(&path);
    }

    {
        path_t *path = path_new("/aa/bb/cc");
        assert(path_is_absolute(path));
        assert(string_equal(path_string(path), "/aa/bb/cc"));
        path_destroy(&path);
    }

    {
        path_t *path = path_new("a/b/c");
        assert(string_equal(path_string(path), "a/b/c"));
        path_join(path, "d/e/f");
        assert(string_equal(path_string(path), "a/b/c/d/e/f"));
        path_destroy(&path);
    }

    {
        path_t *path = path_new("a/b/c");
        assert(string_equal(path_string(path), "a/b/c"));
        path_join(path, "/d/e/f");
        assert(string_equal(path_string(path), "a/b/c/d/e/f"));
        path_destroy(&path);
    }

    {
        path_t *path = path_new("/a/b/c");
        assert(string_equal(path_string(path), "/a/b/c"));
        path_join(path, "/d/e/f");
        assert(string_equal(path_string(path), "/a/b/c/d/e/f"));
        path_destroy(&path);
    }

    {
        path_t *path = path_new("/a/b/c");
        assert(string_equal(path_string(path), "/a/b/c"));
        path_join(path, "../../d/e/f");
        assert(string_equal(path_string(path), "/a/d/e/f"));
        path_destroy(&path);
    }

    {
        path_t *x = path_new("/a/b/c");
        path_t *y = path_new("/a/b/c/d/../d/../");
        assert(path_equal(x, y));
        path_destroy(&x);
        path_destroy(&y);
    }

    {
        path_t *x = path_new("../a/..");
        path_t *y = path_new("..");
        assert(path_equal(x, y));
        path_destroy(&x);
        path_destroy(&y);
    }

    {
        path_t *x = path_new("../a/../../b/..");
        path_t *y = path_new("../..");
        assert(path_equal(x, y));
        path_destroy(&x);
        path_destroy(&y);
    }

    {
        path_t *x = path_new("/a/b/c");
        path_t *y = path_new("////a/////b/////c/////");
        assert(path_equal(x, y));
        path_destroy(&x);
        path_destroy(&y);
    }

    {
        path_t *x = path_new("/a/b/c");
        path_t *y = path_new("/./././a/./././////b/././././c/././");
        assert(path_equal(x, y));
        path_destroy(&x);
        path_destroy(&y);
    }

    {
        path_t *x = path_new("/a/b/c");
        path_t *y = path_new("/a/b/d");
        path_t *z = path_relative(x, y);
        path_t *w = path_new("../d");
        assert(path_equal(z, w));
        path_destroy(&x);
        path_destroy(&y);
        path_destroy(&z);
        path_destroy(&w);
    }

    {
        path_t *x = path_new("/a/b/c");
        path_t *y = path_new("/a/b");
        path_t *z = path_relative(x, y);
        path_t *w = path_new("..");
        assert(path_equal(z, w));
        path_destroy(&x);
        path_destroy(&y);
        path_destroy(&z);
        path_destroy(&w);
    }

    {
        path_t *x = path_new("/a/b");
        path_t *y = path_new("/a/b/c");
        path_t *z = path_relative(x, y);
        path_t *w = path_new("c");
        assert(path_equal(z, w));
        path_destroy(&x);
        path_destroy(&y);
        path_destroy(&z);
        path_destroy(&w);
    }

    test_end();
}
