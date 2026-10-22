#include <stdio.h>
#include <stdlib.h>
#include "memscope.h"

int global_counter = 42;
static double global_ratio = 3.1416;
static char global_label[] = "MemScope demo";

static void inspect_function_scope(void) {
    int local_count = 7;
    long local_offset = 4096L;
    double local_temperature = 21.75;
    char local_message[] = "hello from stack";

    MS_INT(local_count, "Stack");
    MS_LONG(local_offset, "Stack");
    MS_DOUBLE(local_temperature, "Stack");
    MS_CHAR_ARRAY(local_message, "Stack");
}

int main(void) {
    int *heap_values = malloc(3 * sizeof(*heap_values));
    if (!heap_values) {
        fputs("MemScope demo: malloc failed\n", stderr);
        return 1;
    }
    heap_values[0] = 3;
    heap_values[1] = 6;
    heap_values[2] = 9;

    MS_INT(global_counter, "Global/Data");
    MS_DOUBLE(global_ratio, "Global/Data");
    MS_CHAR_ARRAY(global_label, "Global/Data");
    inspect_function_scope();

    char heap_display[128];
    snprintf(heap_display, sizeof(heap_display), "[%d, %d, %d]", heap_values[0], heap_values[1], heap_values[2]);
    ms_register("heap_values", "int[3] allocation", heap_display,
                heap_values, 3 * sizeof(*heap_values), "Heap");

    ms_print_json();
    free(heap_values);
    return 0;
}
