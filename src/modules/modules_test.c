#include "index.h"

void
modules_test(void) {
    printf("<modules_test>\n");

    memory_test();
    char_test();
    string_test();
    list_test();
    hash_test();
    set_test();
    vec_test();
    blob_test();
    file_test();
    path_test();
    array_test();
    stack_test();
    int_test();
    code_test();
    utf8_test();
    text_test();
    font_test();
    lexer_test();
    sexp_test();
    canvas_test();
    image_test();
    store_test();
    time_test();
    thread_test();
    queue_test();

    printf("</modules_test>\n");
}
