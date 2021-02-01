#include <stdio.h>

#include "ltk.h"
#include "tex.h"

void usage();
static char global_buffer[KILO] = {0};
static String_View gb = {global_buffer, KILO};

Yml yml;
int main() {
    String_View file = readfile("./t1.md");
    
    bool hasit = has_yml(file);
    yml_date(&yml, file);
    yml_letter(&yml, file);
    yml_type(&yml, file);
    yml_title(&yml, file);
    yml_author(&yml, file);

    sv_free(file);
    return 0;
}

void usage() {
#if 0 
    Need to parse Yml with the following fields:
        Bool date: true
        enum note type: notes
        int8_t letter_size: 12
        String_View title: My awesome notes
        String_View author: My awesome name
#endif
    const char *u = 
    "ltk - Markdown LaTeX converter\n"
    "ltk parses the markdown and compiles the latex using pdflatex\n"
    "Available packages:\n"
    "\\usepackage[a4paper, margin=2cm]{geometry}\n"
    "\\usepackage[utf8]{inputenc}\n"
    "\\usepackage[T1]{fontenc}\n"
    "\\usepackage{hyperref}\n"
    "\\usepackage{amsmath}\n"
    "\\usepackage{amsfonts}\n"
    "\\usepackage{amssymb}\n"
    "\\usepackage{amsthm}\n"
    "\\usepackage{amsbsy}\n"
    "\\usepackage{mathrsfs}\n"
    "\\usepackage{slashed}\n"
    "\\usepackage{graphicx}\n"
    "\\usepackage{fancyhdr}\n"
    "\\usepackage{color}\n"
    "\\usepackage{cite}\n"
    "\\usepackage{float}\n";

    printf("%s\n", u);
}



