#include <stdio.h>

// Always need to define the size of buffers
#define BUFFERMAX MEGA

#include "ltk.h"
#include "yml.h"
#include "tex.h"

void usage(FILE *stream);
void mount_tex_file();

// Default parsing config
static
Yml yml = {
    .date   = false,
    .toc    = false,
    .type   = NOTE,
    .letter = {"10", 2},
    .title  = {NULL, 0},
    .author = {NULL, 0}
};

const char *next(size_t *argc, char ***args) {
    if (*argc > 0) {
        const char *instruction = *args[0];
        *args = *args + 1;
        *argc = *argc - 1;
        return instruction; 
    } else {
        return NULL;
    }
}

int main(size_t argc, char **argv) {

        String_View input = sv(argv[1]);
        mount_tex_file(input);
       
#if 0 
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

    next(&argc, &argv); 
    while (argc) {
        const char *inst = next(&argc, &argv);
        if (strcmp(inst, "--pdf") == 0) {
            printf("This is inst: %s\n", inst);
        }
    }
#endif

    return 0;
}
 

void mount_tex_file(String_View md_file) {

    String_View md_contents = readfile(md_file);
    yml_parse(&yml, &md_contents);
    
    FILE *out  = tex_openfile(md_file);
    tex_init(&yml, out);
    tex_parse(sv_trim_left(md_contents), out);
    
    fclose(out);
}


void usage(FILE *stream) {
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

    fprintf(stream, "%s\n", u);
}



