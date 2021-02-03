#include <stdio.h>

// Always need to define the size of buffers
#define BUFFERMAX MEGA

#include "ltk.h"
#include "yml.h"
#include "tex.h"

void usage();

static char global_buffer[BUFFERMAX] = {0};
static char holder_buffer[BUFFERMAX] = {0};
static String_View   hb = {holder_buffer, BUFFERMAX};
static String_View   gb = {global_buffer, BUFFERMAX};
static String_View *pgb = &gb;
static String_View *phb = &hb;

static
Yml yml = {
    .date   = false,
    .toc    = true,
    .type   = NOTE,
    .letter = {"10", 2},
    .title  = {NULL, 0},
    .author = {NULL, 0}
};

static
Cmd cmd = {
    .md_open   = (String_View) {NULL, 0},
    .md_close  = (String_View) {NULL, 0},
    .tex_open  = (String_View) {NULL, 0},
    .tex_close = (String_View) {NULL, 0},
};

FILE *open_tex(String_View md_filename) {
        String_View file_name = sv_chop_str(&md_filename, sv(".md")); 
        sv_append_in_buffer(pgb, file_name, sv(".tex"));
        FILE *out = fopen(gb.str, "w");
        return out;
}

int main(size_t argc, char **argv) {


    if (argc == 2) {
        String_View md_file = sv(argv[1]);

        String_View file = readfile(md_file);
        FILE *out = open_tex(md_file);

        yml_parse(&yml, &file);
        tex_init(&yml, out);
        
        String_View md = sv_trim_left(file);
        tex_parse(pgb, phb, &cmd, md);
        fprintf(out, SVFMT, SVARG(gb));
//        tex_end(out);
        
        fclose(out);
    }

    
#if 0
    String_View file = readfile("../tests/bold_it.md");

    parse_in_buffer(pgb, file, cmd_bf(&cmd));
    sv_copy(phb, pgb);
    parse_in_buffer(pgb, hb, cmd_it(&cmd));
    sv_print(gb);

    sv_free(file);

    yml_parse(&yml, file);
    FILE *fout = fopen("out.tex", "w");
    tex_init(&yml, fout);
    fclose(fout);
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



