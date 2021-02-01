#include <stdio.h>
#include <unistd.h>

#include "ltk.h"

#if 0 
#define MAX_CAPACITY (1 * KILO)
uint8_t membuffer[MAX_CAPACITY];
Memory memory = { .capacity = MAX_CAPACITY,
                  .size     = 0,
                  .buffer   = membuffer };
#endif

int main() {
    String_View s = sv(
    "This is a *test sentence* with bold and *this is another bold*."
    );

    char global_buffer[MAXSTR] = {0};
    String_View gb = {global_buffer, MAXSTR};

    parse_in_buffer(&gb, s, sv("*"));
    printsv(gb);

 #if 0
    String_View file = readfile("./t1.md");
#endif

    return 0;
}


