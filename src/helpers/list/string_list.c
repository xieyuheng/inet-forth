#include "index.h"

list_t *
string_list_new(void) {
    return list_new_with((destroy_fn_t *) string_destroy);
}

list_t *
string_list_copy(list_t *list) {
    list_set_copy_fn(list, (copy_fn_t *) string_copy);
    return list_copy(list);
}

void
string_list_print(list_t *list, const char *delimiter, file_t *file) {
    void *value = list_first(list);
    while (value) {
        if (list_cursor_is_end(list)) {
            fprintf(file, "%s", (char *) value);
        } else {
            fprintf(file, "%s%s", (char *) value, delimiter);
        }

        value = list_next(list);
    }
}
