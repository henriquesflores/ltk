#ifndef LTK_H
#define LTK_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define VEC_INCREASE 10
#define INITIAL_CAPACITY 20
#define MAXPATH 256
#define MAXSTR (2 * 1024)

typedef struct String_View {
    const char *str;
    size_t len;
} String_View;

#define SVFMT "%.*s"
#define SVARG(sv) (int) sv.len, sv.str

String_View sv(const char *cstr) {
    size_t N  = strlen(cstr);
    return (String_View) {cstr, N};
}

void printsv(String_View sv) {
    printf(SVFMT"\n", SVARG(sv));
}

void sv_free(String_View *str) {
    free((char *)str->str);
}

String_View sv_trim_left(String_View sv) {
    size_t i = 0;
    while (i < sv.len && isspace(sv.str[i])) {
        i++;
    }

    return (String_View) {sv.str + i, sv.len - i};
}

String_View sv_trim_right(String_View sv) {
    size_t i = 0;
    while (i < sv.len && isspace(sv.str[sv.len - i - 1])) {
        i++;
    }

    return (String_View) {sv.str, sv.len - i};
}

String_View sv_trim(String_View sv) {
    return sv_trim_left(sv_trim_right(sv));
}

String_View readfile(const char *filepath) {
    FILE *f = fopen(filepath, "r");

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *contents = malloc(file_size + 1);

    size_t bytes_read =
        fread(contents, 1, file_size, f); 

    assert(bytes_read == file_size);
    contents[file_size] = 0;
    fclose(f);
    
    return (String_View) {contents, file_size};
}

size_t sv_count_char(String_View sv, char c) {
    int i = 0;
    for (size_t j = 0; j < sv.len; j++) {
        i += sv.str[j] == c ? 1 : 0;
    }

    return i;
}

int sv_find_char(String_View sv, char c) {
    int i = 0;
    while (i <= sv.len && sv.str[i] != c) {
        i += 1;
    }

    return (i > sv.len) ? -1 : i;
}

String_View sv_chop_char(String_View *sv, char c) {
    int i = sv_find_char(*sv, c);

    if (i == -1) return *sv;

    String_View choped = {sv->str, i};

    sv->str = sv->str + i + 1;
    sv->len = sv->len - i - 1;

    return choped;
}

int sv_find_str(String_View sv, String_View find) {
    size_t N = sv.len - find.len + 1;
    for (size_t j = 0; j < N; j++) {
        int needle = memcmp(find.str, sv.str + j, find.len);

        if (!needle) 
            return j;
    }

    return -1; 
}

String_View sv_chop_str(String_View *sv, String_View find) {
    int index = sv_find_str(*sv, find);

    if (index == -1) return *sv;

    String_View result = {sv->str, index};
    sv->str = sv->str + index + find.len;
    sv->len = sv->len - index - find.len;

    return result;
}

String_View *sv_replace_in_buffer(String_View *buffer,
                                  String_View string,
                                  String_View find,
                                  String_View replace) {

    int N = MAXSTR - string.len - replace.len + find.len;
    assert(N > 0);

    String_View choped = sv_chop_str(&string, find);
    buffer->len = choped.len + replace.len + string.len;
    sprintf((char *)buffer->str, SVFMT SVFMT SVFMT, SVARG(choped), SVARG(replace), SVARG(string));
    return buffer;
}

String_View *parse_in_buffer(String_View *buffer,
                             String_View str,
                             char marker) {

    size_t nm = sv_count_char(str, marker); 
    String_View m = {(const char *) &marker, 1};

    while (nm) {
        if (nm-- % 2 == 0) 
            sv_replace_in_buffer(buffer, str, m, sv("\\textbf{"));
        else 
            sv_replace_in_buffer(buffer, str, m, sv("}"));
    
        printf(SVFMT"\n", (int) buffer->len, buffer->str);
        printf("-------\n");
        str = *buffer;
    } 

    return buffer;
}

String_View exepath() {
    char *path = malloc(MAXPATH); 
    int bytes = readlink("/proc/self/exe", path, MAXPATH);
    assert(bytes != -1);
    path[bytes] = 0;
    return (String_View) {path, strlen(path)};
}

#endif // LTK_H
