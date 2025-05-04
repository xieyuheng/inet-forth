#include "index.h"

void
test_file(void) {
    test_start();

    char *base = dirname(string_copy(__FILE__));
    char *file_name = string_append(base, "/abc.txt");


    {
        file_t *file = file_open_or_fail(file_name, "r");
        char *string = file_read_string(file);
        assert(
            string_equal(
                string,
                "abc\n"
                "abc\n"
                "abc\n"));
    }

    {
        file_t *file = file_open_or_fail(file_name, "r");
        blob_t *blob = file_read_blob(file);
        assert(
            string_equal(
                blob_string(blob),
                "abc\n"
                "abc\n"
                "abc\n"));
    }

    test_end();
}
