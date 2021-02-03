#ifndef LTK_H
#define LTK_H

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define KILO (1024LL)
#define MEGA (KILO * 1024LL)
#define GIGA (MEGA * 1024LL)

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

void sv_print(String_View sv) {
    printf(SVFMT"\n", SVARG(sv));
}

void sv_fprintf(FILE *stream, String_View sv) {
    assert(stream);
    fprintf(stream, SVFMT, SVARG(sv));
}

void sv_free(String_View str) {
    free((char *)str.str);
}

String_View *sv_copy(String_View *dest, String_View *src) {
    assert(dest);
    assert(src);

    dest->len = src->len;
    memcpy((char *)dest->str, src->str, src->len);

    return dest;
}

bool sv_eq(String_View a, String_View b) {
    if (a.len != b.len)
        return false;
    else if (!memcmp(a.str, b.str, a.len)) 
        return true; 
    else 
        return false;
}

bool sv_isempty(String_View sv) {
    return sv.len == 0 ? true : false;
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
    if (!f) {
        fprintf(stderr, "Cannot find file: %s\n", filepath);
        exit(1);
    }

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
    while (i < sv.len && sv.str[i] != c) {
        i += 1;
    }

    return (i >= sv.len) ? -1 : i;
}

String_View sv_chop_char(String_View *sv, char c) {
    int i = sv_find_char(*sv, c);

    if (i == -1) return *sv;

    String_View choped = {sv->str, i + 1};

    sv->str = sv->str + i + 1;
    sv->len = sv->len - i - 1;

    return choped;
}

String_View sv_getline(String_View *sv) {
    return sv_chop_char(sv, '\n');
}

size_t sv_count_str(String_View src, String_View find) {
    size_t i = 0;
    
    int N = src.len - find.len + 1;
    if (N < 0) return i;

    for (int j = 0; j < N; j++) {
        int needle = memcmp(find.str, src.str + j, find.len);
        i += (needle == 0) ? 1 : 0;
    }

    return i;
}

int sv_find_str(String_View sv, String_View find) {
    int N = sv.len - find.len + 1;
    if (N < 0)
        return -1;

    for (int j = 0; j < N; j++) {
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

String_View *sv_append_in_buffer(String_View *buffer, 
                                 String_View sv1, 
                                 String_View sv2) {

    assert(buffer->len >= sv1.len + sv2.len);
    buffer->len = sv1.len + sv2.len;
    snprintf( (char *)buffer->str
            , buffer->len + 1
            , SVFMT SVFMT
            , SVARG(sv1), SVARG(sv2)
            );

    return buffer;
}

String_View *sv_replace_in_buffer(String_View *buffer,
                                  String_View string,
                                  String_View find,
                                  String_View replace) {
    
    String_View choped = sv_chop_str(&string, find);    
    if (sv_eq(choped, string))
        return buffer;

    buffer->len = choped.len + replace.len + string.len;
    snprintf( (char *)buffer->str
            , buffer->len + 1
            , SVFMT SVFMT SVFMT
            , SVARG(choped)
            , SVARG(replace)
            , SVARG(string)
            ); 

    return buffer;
}

String_View *exepath(String_View *path) {
    int bytes = readlink("/proc/self/exe", (char *)path->str, path->len);
    assert(bytes != -1);
    return path;
}

#endif // LTK_H
