#include <stdio.h>

#include "ltk.h"

#if 0 
#define MAX_CAPACITY (1 * KILO)
uint8_t membuffer[MAX_CAPACITY];
Memory memory = { .capacity = MAX_CAPACITY,
                  .size     = 0,
                  .buffer   = membuffer };
#endif

int main() {
    const char *s = "This is a *test sentence* with bold.";
    String_View w = sv(s);

    printf(SVFMT"\n", SVARG(w));
 
    String_View piece; 
    while (sv_consume_char(&piece, &w, '*')) {
        printf(SVFMT"\n", SVARG(piece));
    }


#if 0
    String_View file = readfile("./test.md");
    Vector a = sv_find(file, '$');
#endif

    return 0;
}


