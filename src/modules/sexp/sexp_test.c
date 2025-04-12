#include "index.h"

void
sexp_test(void) {
    test_start();


    sexp_print(sexp_parse("(a b c)"), stdout);
    printf("\n");

    sexp_print(sexp_parse("((a \"b\" c) (a . c) (1 1.2))"), stdout);
    printf("\n");

    test_end();
}
