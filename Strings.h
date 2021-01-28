#ifndef STRINGS_H
#define STRINGS_H

#include "Memory.h"

typedef struct String_View {
    const char *str;
    size_t len;
} String_View;

String_View string(const char *cstr) {
    size_t N  = strlen(cstr);
    char *str = memory_alloc(&memory, N);
    memory_copy(str, cstr, N);
    return (String_View) {str, N};
}

// TODO: this free string does not work.
void free_string(String_View *str) {
    memory_free(&memory, str->len);
}

const char *sv_to_cstr(String_View *sv) {
    char *cstr = memory_alloc(&memory, sv->len + 1);
    memory_copy(cstr, sv->str, sv->len);
    cstr[sv->len] = 0;
    return (const char *) cstr;
}

void printsv(String_View *sv) {
    for (size_t i = 0; i < sv->len; i++) 
        printf("%c", sv->str[i]);
    printf("\n");
}

#endif // STRINGS_H
