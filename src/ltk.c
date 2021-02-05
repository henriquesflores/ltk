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

char *next(size_t *argc, char ***args) {
    if (*argc > 0) {
        char *instruction = *args[0];
        *args = *args + 1;
        *argc = *argc - 1;
        return instruction; 
    } else {
        return NULL;
    }
}

int main(size_t argc, char **argv) {
        
        bool set_pdf = false;
        char *inputfile;

        if (argc == 1)
            usage(stderr);

        next(&argc, &argv); 
        while (argc) {
            char *inst = next(&argc, &argv);
            if (argc == 0) {
                inputfile = inst;
                mount_tex_file(sv(inputfile));
            } else if (strcmp(inst, "--pdf") == 0) {
                set_pdf = true;
            }
        }

        
        if (set_pdf) {
            String wd;
            getcwd(wd.str, BUFFERMAX);
            wd.len = strlen(wd.str);

            String tf;
            str_replace(&tf, sv(inputfile), sv(".md"), sv(".tex"));

            String final;
            str_append(&final, sv_from_string(&wd), sv("/"));
            str_copy(&wd, &final);
            str_append(&final, sv_from_string(&wd), sv_from_string(&tf));
            execl("/usr/bin/pdflatex", "/usr/bin/pdflatex", str_cstr(&final), (char *) NULL);
       }

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



