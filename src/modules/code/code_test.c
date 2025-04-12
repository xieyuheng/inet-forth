#include "index.h"

void
code_test(void) {
    test_start();

    {
        const char *string = ""
            ".\n"
            "12345\n"
            ".";

        assert(code_max_lineno(string) == 3);
    }

    {
        const char *string = ""
            ".\n"
            "12345\n"
            ".\n";

        assert(code_max_lineno(string) == 4);
    }

    {
        const char *string = ""
            ".\n"
            "12345\n"
            ".\n";

        assert(code_lineno_of_index(string, 0) == 1);
        assert(code_lineno_of_index(string, 1) == 1);
        assert(code_lineno_of_index(string, 100) == 4);
    }

    test_end();
}
