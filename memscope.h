#ifndef MEMSCOPE_H
#define MEMSCOPE_H
#include <stddef.h>
#include <stdio.h>

void ms_register(const char *name, const char *type, const char *value,
                 const void *address, size_t size, const char *region);
void ms_print_json(void);

#define MS_INT(var, region_name) do { \
    char ms_value_buf[64]; \
    snprintf(ms_value_buf, sizeof(ms_value_buf), "%d", (var)); \
    ms_register(#var, "int", ms_value_buf, &(var), sizeof(var), (region_name)); \
} while (0)

#define MS_LONG(var, region_name) do { \
    char ms_value_buf[64]; \
    snprintf(ms_value_buf, sizeof(ms_value_buf), "%ld", (var)); \
    ms_register(#var, "long", ms_value_buf, &(var), sizeof(var), (region_name)); \
} while (0)

#define MS_DOUBLE(var, region_name) do { \
    char ms_value_buf[64]; \
    snprintf(ms_value_buf, sizeof(ms_value_buf), "%.4f", (var)); \
    ms_register(#var, "double", ms_value_buf, &(var), sizeof(var), (region_name)); \
} while (0)

#define MS_CHAR_ARRAY(var, region_name) \
    ms_register(#var, "char[]", (var), (var), sizeof(var), (region_name))

#endif
