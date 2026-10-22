#include <stdio.h>
#include <string.h>
#include "memscope.h"

#define MAX_ENTRIES 128
#define TEXT_SIZE 256

typedef struct {
    char name[TEXT_SIZE];
    char type[TEXT_SIZE];
    char value[TEXT_SIZE];
    char address[64];
    char region[TEXT_SIZE];
    size_t size;
} MemoryEntry;

static MemoryEntry entries[MAX_ENTRIES];
static size_t entry_count = 0;

static void copy_text(char *dst, size_t cap, const char *src) {
    if (!src) src = "";
    snprintf(dst, cap, "%s", src);
}

void ms_register(const char *name, const char *type, const char *value,
                 const void *address, size_t size, const char *region) {
    if (entry_count >= MAX_ENTRIES) return;
    MemoryEntry *entry = &entries[entry_count++];
    copy_text(entry->name, sizeof(entry->name), name);
    copy_text(entry->type, sizeof(entry->type), type);
    copy_text(entry->value, sizeof(entry->value), value);
    copy_text(entry->region, sizeof(entry->region), region);
    snprintf(entry->address, sizeof(entry->address), "%p", address);
    entry->size = size;
}

static void json_string(const char *text) {
    putchar('"');
    for (const unsigned char *p = (const unsigned char *)text; *p; ++p) {
        switch (*p) {
            case '"': fputs("\\\"", stdout); break;
            case '\\': fputs("\\\\", stdout); break;
            case '\n': fputs("\\n", stdout); break;
            case '\r': fputs("\\r", stdout); break;
            case '\t': fputs("\\t", stdout); break;
            default:
                if (*p < 0x20) printf("\\u%04x", *p);
                else putchar(*p);
        }
    }
    putchar('"');
}

void ms_print_json(void) {
    puts("{\"entries\":[");
    for (size_t i = 0; i < entry_count; ++i) {
        MemoryEntry *e = &entries[i];
        if (i) puts(",");
        printf("{\"name\":"); json_string(e->name);
        printf(",\"type\":"); json_string(e->type);
        printf(",\"value\":"); json_string(e->value);
        printf(",\"address\":"); json_string(e->address);
        printf(",\"region\":"); json_string(e->region);
        printf(",\"size\":%zu}", e->size);
    }
    puts("]}");
}
