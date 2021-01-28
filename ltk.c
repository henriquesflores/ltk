#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Strings.h"
#include "Memory.h"

#define MAX_CAPACITY (1 * KILO)
uint8_t membuffer[MAX_CAPACITY];
Memory memory = { .capacity = MAX_CAPACITY,
                  .size     = 0,
                  .buffer   = membuffer };

int main() {

    const char *s = "This is *a bold text*";
    String_View sv = string(s);
    printsv(&sv); 
    String_View muts = string("Does this work?");
    printsv(&muts);
    const char *cmuts = sv_to_cstr(&muts);
    printf("%s\n", cmuts);
     

    return 0;
}

