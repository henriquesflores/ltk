#include <stdio.h>

#include "ltk.h"

static char global_buffer[KILO] = {0};
static String_View gb = {global_buffer, KILO};

int main() {
    String_View s = sv(
        "This is a *test sentence* with bold and *this is another bold*. "
        "But if I continue to write *random* bold messages, does it "
        "still work? I wonder... *Maybe...*"
    );
    
    parse_in_buffer(&gb, s, sv("*"));
    printsv(gb);

#if 0
    String_View file = readfile("./test.md");
    sv_free(file);
#endif


    return 0;
}


