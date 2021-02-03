#include <stdio.h>

// Always need to define the size of buffers
#define BUFFERMAX MEGA

#include "ltk.h"
#include "yml.h"
#include "tex.h"

void usage();
void mount_tex_file();

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


int main(size_t argc, char **argv) {

    if (argc == 2) {
        String_View input = sv(argv[1]);
        mount_tex_file(input);

        getcwd((char *)hb.str, KILO);
        hb.len = strlen(hb.str);

        String_View filename = sv_chop_str(&input, sv(".md"));
        char tt[KILO];
        String_View texfile = {tt, KILO};
        sv_append_in_buffer(&texfile, filename, sv(".tex"));
        sv_append_in_buffer(pgb, hb, sv("/"));
        sv_copy(phb, pgb);
        sv_append_in_buffer(pgb, hb, texfile);
        execl("/usr/bin/pdflatex", "/usr/bin/pdflatex", gb.str, (char *) NULL);
    }

    printf(SVFMT, SVARG(hb));


    return 0;
}

void mount_tex_file(String_View md_file) {
    String_View file = readfile(md_file);
    char *ptr_tofree = (char *)file.str;
    
    FILE *out = tex_openfile(md_file);
    yml_parse(&yml, &file);
    tex_init(&yml, out, pgb, phb);
    
    String_View md = sv_trim_left(file);
    tex_parse(pgb, phb, &cmd, md);
    fprintf(out, SVFMT, SVARG(gb));
    tex_end(out);
    
    fclose(out);
    free(ptr_tofree);
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



