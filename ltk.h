#ifndef LTK_H
#define LTK_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct Vector {
    uint32_t *elem;
    size_t size;
    size_t capacity;
} Vector;

Vector vector(size_t capacity);
Vector *vec_push(Vector *vec, uint32_t i);
void vec_free(Vector *vec);

typedef struct String_View {
    const char *str;
    size_t len;
} String_View;

#define SVFMT "%.*s"
#define SVARG(sv) (int) sv.len, sv.str

String_View sv(const char *cstr);
String_View readfile(const char *filepath);
String_View sv_chop_char(String_View *sv, char c);
void sv_free(String_View *sv);
Vector sv_find_all(String_View sv, char c);

// ---------------------------------------
Vector vector(size_t capacity) {
    uint32_t *elem = malloc(capacity * sizeof(uint32_t));
    memset(elem, 0, capacity * sizeof(uint32_t));
    return (Vector) {elem, 0, capacity};
}

#define INCREASE 10
Vector *vec_push(Vector *vec, uint32_t i) {
    if (vec->capacity >= vec->size + 1) {
        vec->elem[vec->size++] = i;
    } else {
        vec->elem = realloc(vec->elem, (vec->capacity + INCREASE) * sizeof(uint32_t));
        vec->capacity = vec->capacity + INCREASE;
        vec->elem[vec->size++] = i;
    }

    return vec;
}

void vec_free(Vector *vec) {
    free(vec->elem);
}

// ---------------------------------------------------
String_View sv(const char *cstr) {
    size_t N  = strlen(cstr);
    return (String_View) {cstr, N};
}

#define INITIAL_CAPACITY 20
Vector sv_find_all(String_View sv, char c) {
    Vector pos = vector(INITIAL_CAPACITY);

    for (size_t i = 0; i < sv.len; i++) {
       if (sv.str[i] == c) 
           vec_push(&pos, i);
    }

    return pos;
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

int sv_consume_char(String_View *piece, String_View *sv, char c) {
    *piece = sv_chop_char(sv, c);
     return piece != sv ? 0 : 1;
}

#endif // LTK_H
