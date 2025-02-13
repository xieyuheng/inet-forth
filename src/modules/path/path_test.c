#include "index.h"

void
path_test(void) {
    printf("<path_test>\n");

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

    printf("</path_test>\n");
}
