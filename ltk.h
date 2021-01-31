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
#define MAXSTR 256

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

String_View sv_chop_char(String_View *sv, char c) {
    size_t i = 0;
    while (i <= sv->len && sv->str[i] != c) {
        i++;
    }

    if (i > sv->len)
        return *sv;

    String_View choped = {sv->str, i};

    sv->str = sv->str + i + 1;
    sv->len = sv->len - i - 1;

    return choped;
}

size_t sv_count_char(String_View sv, char c) {
    int i = 0;
    for (size_t j = 0; j < sv.len; j++) {
        i += sv.str[j] == c ? 1 : 0;
    }

    return i;
}

int sv_find_str(String_View sv, String_View find) {
    size_t N = sv.len - find.len + 1;
    if (N < 0)
        return -1; 

    for (size_t j = 0; j < N; j++) {
        int needle = memcmp(find.str, sv.str + j, find.len);

        if (!needle) 
            return j;
    }

    return -1; 
}

String_View sv_chop_str(String_View *sv, String_View find) {
    int index = sv_find_str(*sv, find);

    if (index == -1)
        return *sv;

    String_View result = {sv->str, index};
    sv->str += index + find.len;
    sv->len -= index + find.len;

    return result;
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

String_View sv_replace_str(String_View sv, 
                           String_View find,
                           String_View replace) {

    String_View choped = sv_chop_str(&sv, find);
   
    size_t new_len = choped.len + replace.len + sv.len;
    char *new = malloc(new_len);
    sprintf(new, SVFMT SVFMT SVFMT, SVARG(choped), SVARG(replace), SVARG(sv));

    return (String_View) {new, new_len};
}

String_View *sv_replace_in_buffer(String_View *buffer,
                                  String_View string,
                                  String_View find,
                                  String_View replace) {

    size_t N = buffer->len - string.len - replace.len + find.len;
    assert(N >= 0);

    String_View choped = sv_chop_str(&string, find);
    sprintf((char *)buffer->str, SVFMT SVFMT SVFMT, SVARG(choped), SVARG(replace), SVARG(string));
    buffer->len = choped.len + replace.len + string.len;
    return buffer;
}

String_View parse_element(String_View str, char element) {
    size_t nelem = sv_count_char(str, element); 
    
    String_View el = {(const char *) &element, 1};
    char *new = malloc(MAXSTR);
    
    String_View u = {new, MAXSTR};
    do {
        if (nelem % 2 == 0) 
            sv_replace_in_buffer(&u, str, el, sv("\\textbf{"));
        else 
            sv_replace_in_buffer(&u, str, el, sv("}"));

        str = u;
    } while(--nelem);

    return u;
}

String_View exepath() {
    char *path = malloc(MAXPATH); 
    int bytes = readlink("/proc/self/exe", path, MAXPATH);
    assert(bytes != -1);
    path[bytes] = 0;
    return (String_View) {path, strlen(path)};
}

#endif // LTK_H
