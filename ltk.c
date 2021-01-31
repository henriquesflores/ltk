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
    "Continuing like this we obtain another *bold sentence* that"
    "could generate *errors*. Maybe... who knows"
    );

    char buffer[MAXSTR];
    String_View sv_buffer = {buffer, MAXSTR};
    parse_in_buffer(&sv_buffer, s, '*');
    printsv(s);
    printsv(sv_buffer);

 #if 0
    String_View file = readfile("./t1.md");
#endif

    return 0;
}


