#include <stdio.h>

// Always need to define the size of buffers
#define BUFFERMAX MEGA

#include "ltk.h"
#include "yml.h"
#include "tex.h"

void usage(FILE *);
void mount_tex_file(String_View);
void mount_pdf(String_View, bool);

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

char *next(int *argc, char ***args) {
    if (*argc > 0) {
        char *instruction = *args[0];
        *args = *args + 1;
        *argc = *argc - 1;
        return instruction; 
    } else {
        return NULL;
    }
}

int main(int argc, char **argv) {
        
        bool set_pdf = false;
        char *inputfile;

        if (argc == 1) {
            usage(stderr);
            exit(0);
        }

        next(&argc, &argv); 
        while (argc) {
            char *inst = next(&argc, &argv);
            if (argc == 0) {
                inputfile = inst;
            } else if (strcmp(inst, "--pdf") == 0) {
                set_pdf = true;
            }
        }

        mount_tex_file(sv(inputfile));
        mount_pdf(sv(inputfile), set_pdf);

        return 0;
}

void mount_pdf(String_View input, bool set_pdf) {
    if (set_pdf) {
            String wd;
            getcwd(wd.str, BUFFERMAX);
            wd.len = strlen(wd.str);
            
            String final;
            {
                String tf;
                str_replace(&tf, input, sv(".md"), sv(".tex"));

                str_append(&final, sv_from_string(&wd), sv("/"));
                str_copy(&wd, &final);
                str_append(&final, sv_from_string(&wd), sv_from_string(&tf));
            }
           
            execl("/usr/bin/pdflatex", "/usr/bin/pdflatex", str_cstr(&final), (char *) NULL);
    } else {
        return;
    }
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
    "LTK - Markdown LaTeX converter:\n"
    "Usage:\n"
    "\tltk [options] [file]\n"
    "Options:\n"
    "\t--pdf        Generates pdf file\n"
    "\n"
    "DEPENDENCIES:\n"
    "LTK parses markdown syntax and compiles LaTeX using pdflatex.\n"
    "You need to have pdflatex installed to use --pdf option.\n"
    "\n"
    "MARKDOWN FORMAT:\n"
    "LTK looks for a yaml that can be placed anywhere in the document.\n"
    "It parses only the following fields:\n"
    "\ttitle: String and defaults to empty; \n"
    "\tauthor: String and defaults to empty;\n" 
    "\ttype: note or article and defaults to note;\n"
    "\tletter: integer and defaults to 10;\n"
    "\tdate: true or false and defaults to false;\n"
    "\ttoc: true or false and defaults to false.\n"
    "These can be put at any order and any extra fields will not be read.\n"
    "\n"
    "LTK writes by default the following list of packages:\n"
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
    "\t\\usepackage{float}\n"
    "The ability to write more packages is at the moment not possible.";

    fprintf(stream, "%s\n", u);
}



