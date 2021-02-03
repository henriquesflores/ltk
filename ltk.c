#include <stdio.h>

#include "ltk.h"
#include "yml.h"
#include "tex.h"

void usage();

static char global_buffer[KILO] = {0};
static String_View gb = {global_buffer, KILO};

static
Yml yml = {
    .date   = false,
    .toc    = true,
    .type   = NOTE,
    .letter = {"10", 2},
    .title  = {NULL, 0},
    .author = {NULL, 0}
};

int main(size_t argc, char **argv) {

    String_View file = readfile("./tests/yml.md");
    yml_parse(&yml, file);

    FILE *fout = fopen("out.tex", "w");
    tex_init(&yml, fout);

    sv_free(file);
    fclose(fout);

#if 0
#endif
    
    return 0;
}

void usage() {
    // TODO: Write complete documentation. 
        const char *u = 
    "ltk - Markdown LaTeX converter:\n"
    "ltk parses the markdown and compiles the latex using pdflatex\n"
    "Need to parse Yml with the following fields:\n"
    "   Bool date: true\n"
    "   enum note type: notes\n"
    "   int8_t letter_size: 12\n"
    "   String_View title: empty\n"
    "   String_View author: empty\n" 
    "Available packages:\n"
    "\t\\usepackage[a4paper, margin=2cm]{geometry}\n"
    "\t\\usepackage[utf8]{inputenc}\n"
    "\t\\usepackage[T1]{fontenc}\n"
    "\t\\usepackage{hyperref}\n"
    "\t\\usepackage{amsmath}\n"
    "\t\\usepackage{amsfonts}\n"
    "\t\\usepackage{amssymb}\n"
    "\t\\usepackage{amsthm}\n"
    "\t\\usepackage{amsbsy}\n"
    "\t\\usepackage{mathrsfs}\n"
    "\t\\usepackage{slashed}\n"
    "\t\\usepackage{graphicx}\n"
    "\t\\usepackage{fancyhdr}\n"
    "\t\\usepackage{color}\n"
    "\t\\usepackage{cite}\n"
    "\t\\usepackage{float}\n";

    puts(u);
}



