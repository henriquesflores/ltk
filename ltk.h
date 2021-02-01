#ifndef LTK_H
#define LTK_H

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
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

void printsv(String_View sv) {
    printf(SVFMT"\n", SVARG(sv));
}

void sv_free(String_View str) {
    free((char *)str.str);
}

String_View *sv_copy(String_View *dest, String_View *src) {
    dest->len = src->len;
    memcpy((char *)dest->str, src->str, src->len);

    return dest;
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

int sv_eq(String_View a, String_View b) {
    if (a.len != b.len)
        return 0;
    else if (!memcmp(a.str, b.str, a.len)) 
        return 1; 
    else 
        return 0;
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

String_View sv_getline(String_View *sv) {
    return sv_chop_char(sv, '\n');
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

String_View *parse_in_buffer(String_View *buffer,
                             String_View str,
                             String_View marker) {
    
    size_t nm = sv_count_char(str, *marker.str); 

    if (!nm) {
        sv_copy(buffer, &str);
        return buffer;
    }

    char temp[KILO];
    String_View holder = {temp, KILO};
    sv_copy(&holder, &str);

    while (nm) {
        if (nm-- % 2 == 0) 
            sv_replace_in_buffer(buffer, holder, marker, sv("\\textbf{"));
        else 
            sv_replace_in_buffer(buffer, holder, marker, sv("}"));
    
        sv_copy(&holder, buffer); 
    } 
    
    return buffer;
}

String_View *exepath(String_View *path) {
    int bytes = readlink("/proc/self/exe", (char *)path->str, path->len);
    assert(bytes != -1);
    return path;
}

#endif // LTK_H
