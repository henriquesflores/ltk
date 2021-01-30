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

typedef struct Vector {
    uint32_t *elem;
    size_t size;
    size_t capacity;
} Vector;

typedef struct String_View {
    const char *str;
    size_t len;
} String_View;

#define SVFMT "%.*s"
#define SVARG(sv) (int) sv.len, sv.str

Vector vector(size_t capacity) {
    uint32_t *elem = malloc(capacity * sizeof(uint32_t));
    memset(elem, 0, capacity * sizeof(uint32_t));
    return (Vector) {elem, 0, capacity};
}

Vector *vec_push(Vector *vec, uint32_t i) {
    if (vec->capacity >= vec->size + 1) {
        vec->elem[vec->size++] = i;
    } else {
        vec->elem = realloc(vec->elem, (vec->capacity + VEC_INCREASE) * sizeof(uint32_t));
        vec->capacity = vec->capacity + VEC_INCREASE;
        vec->elem[vec->size++] = i;
    }

    return vec;
}

void vec_free(Vector *vec) {
    free(vec->elem);
}

String_View sv(const char *cstr) {
    size_t N  = strlen(cstr);
    return (String_View) {cstr, N};
}

Vector sv_find_all(String_View sv, char c) {
    Vector indexes = vector(INITIAL_CAPACITY);

    for (size_t i = 0; i < sv.len; i++) {
       if (sv.str[i] == c) 
           vec_push(&indexes, i);
    }

    return indexes;
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

#endif // LTK_H
