#ifndef LTK_H_
#define LTK_H_

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
#define BUFFERMAX MEGA 

typedef struct String_View {
    const char *str;
    size_t len;
} String_View;

typedef struct String {
    char str[BUFFERMAX];
    size_t len;
} String;

#define SVFMT "%.*s"
#define SVARG(sv) (int) sv.len, sv.str
#define SV_PTRARG(psv) (int) psv->len, psv->str

String_View sv(const char *cstr) {
    size_t N  = strlen(cstr);
    return (String_View) {cstr, N};
}

const char *sv_cstr(String_View sv) {
    char *result = malloc(sv.len + 1);
    memcpy(result, sv.str, sv.len);
    result[sv.len] = 0;

    return result;
}

const char *str_cstr(String *s) {
    assert(s->len + 1 <= BUFFERMAX);
    s->str[s->len] = 0;
    return (const char *)s->str;
}

String *str_copy(String *dest, String *src) {
    assert(dest && src);
    
    dest->len = src->len;
    memcpy(dest->str, src->str, src->len);

    return dest;
}

String *str_copysv(String *s, String_View sv) {
    assert(BUFFERMAX >= sv.len);
    s->len = sv.len;
    memcpy(s->str, sv.str, sv.len);
    return s;
}

String_View sv_from_string(String *s) {
    return (String_View) {(const char *)s->str, s->len};
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
    else if (memcmp(a.str, b.str, a.len)) 
        return false; 
    else 
        return true;
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

String_View readfile(String_View filepath) {
    FILE *f = fopen(filepath.str, "r");
    if (!f) {
        fprintf(stderr, "Cannot find file:" SVFMT "\n", SVARG(filepath));
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *contents = malloc(file_size + 1);

    long bytes_read =
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
    int len = (int) sv.len;
    while (i < len && sv.str[i] != c) {
        i += 1;
    }

    return (i >= len) ? -1 : i;
}

String_View sv_chop_char(String_View *sv, char c) {
    int i = sv_find_char(*sv, c);

    if (i == -1) return *sv;

    String_View choped = {sv->str, i};

    sv->str = sv->str + i + 1;
    sv->len = sv->len - i - 1;

    return choped;
}

String_View sv_get_char_from_right(String_View *sv, char c) {
    int i = sv->len - 1;
    while (i >= 0 && sv->str[i] != c) {
        i--;
    }
    
    if (i < 0) return *sv;

    size_t delta = sv->len - i - 1;
    String_View result = {sv->str, sv->len - delta};
    sv->str = sv->str + delta;
    sv->len = delta;

    return result;
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

String_View sv_take_between(String_View *sv, 
                            String_View begin, 
                            String_View end) {

    String_View init = *sv;

    String_View prev  = sv_chop_str(sv, begin);
    if (sv_eq(prev, *sv)) {
        sv = &init;
        return init;
    }

    String_View field = sv_chop_str(sv, end);
    if (sv_eq(field, *sv)) {
        sv = &init;
        return init;
    }

    return sv_trim(field);
}

String *str_append(
        String *buffer, 
        String_View sv1, 
        String_View sv2
) {
    
    assert(BUFFERMAX >= sv1.len + sv2.len);
    buffer->len = sv1.len + sv2.len;
    snprintf( buffer->str
            , buffer->len + 1
            , SVFMT SVFMT
            , SVARG(sv1), SVARG(sv2)
            );

    return buffer;
}

String *str_replace(
        String *buffer,
        String_View string,
        String_View find,
        String_View replace
) {
  
    assert(BUFFERMAX >= string.len - find.len + replace.len); 
    String_View choped = sv_chop_str(&string, find);    
    if (sv_eq(choped, string))
        return buffer;

    buffer->len = choped.len + replace.len + string.len;
    snprintf( buffer->str
            , buffer->len + 1
            , SVFMT SVFMT SVFMT
            , SVARG(choped)
            , SVARG(replace)
            , SVARG(string)
            ); 

    return buffer;
}

String *str_replace_between(
        String *buffer,
        String_View string,
        String_View start,
        String_View end,
        String_View replace
) {

    assert(BUFFERMAX >= string.len + replace.len - start.len - end.len);

    String_View init   = string;
    String_View before = sv_chop_str(&string, start);
    if (sv_eq(before, string)) {
        str_copysv(buffer, init);
        return buffer;
    }
    
    String_View middle = sv_chop_str(&string, end);
    if (sv_eq(middle, string)) {
        str_copysv(buffer, init);
        return buffer;
    }
    
    buffer->len = before.len + string.len + replace.len;
    snprintf( buffer->str
            , buffer->len + 1
            , SVFMT SVFMT SVFMT
            , SVARG(before)
            , SVARG(replace)
            , SVARG(string)
            );

    return buffer;
}

#endif // LTK_H_
