#include "index.h"

array_t *
string_array_new_auto(void) {
    return array_new_auto_with((destroy_fn_t *) string_destroy);
}

void
string_array_print(array_t *array, const char *delimiter, file_t *file) {
    for (size_t i = 0; i < array_length(array); i++) {
        void *value = array_get(array, i);
        if (i + 1 == array_length(array)) {
            fprintf(file, "%s", (char *) value);
        } else {
            fprintf(file, "%s%s", (char *) value, delimiter);
        }
    }
}
