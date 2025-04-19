#include "index.h"

void
sexp_test(void) {
    test_start();

    {
        const char *code = "(a b c)";
        who_printf("code: %s\n", code);
        who_printf("echo: ");
        sexp_print(sexp_parse(code), stdout); printf("\n");
    }

    {
        const char *code = "((a \"b\" c) (a . c) (1 1.2))";
        who_printf("code: %s\n", code);
        who_printf("echo: ");
        sexp_print(sexp_parse(code), stdout); printf("\n");
    }

    test_end();
}
