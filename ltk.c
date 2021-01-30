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
    String_View nw = sv_replace_str(w, sv("*"), sv("\\textbf{"));
    
    printf(SVFMT, SVARG(nw));

#if 0
    String_View file = readfile("./test.md");
    size_t hm = sv_count_char(file, '$');
    printf("%ld\n", hm);
#endif

    return 0;
}


