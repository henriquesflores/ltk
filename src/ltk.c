#include <stdio.h>

#include "ltk.h"
#include "yml.h"
#include "tex.h"
#include "template.h"

void usage(FILE *);
void mount_tex_file(String_View, String_View);
void mount_pdf(String_View);
char *next(int *, char ***);

#define PATH_SEP '/'
#define TEX_COMPILER "usr/bin/pdflatex"

static
Yml yml = {
    .date   = false,
    .toc    = false,
    .type   = NOTE,
    .letter = {"10", 2},
    .title  = {NULL, 0},
    .author = {NULL, 0}
};

int main(int argc, char **argv) {
        
        bool set_pdf = false;
        String_View inputfile;
        String_View auxfile = {NULL, 0};

        if (argc == 1)
            usage(stderr);

        next(&argc, &argv); 
        while (argc) {
            char *inst = next(&argc, &argv);
            if (strcmp(inst, "--pdf") == 0) {
                set_pdf = true;
            } else if (strcmp(inst, "-g") == 0) {
                dump_template(stderr);  
            } else if (strcmp(inst, "-a") == 0) {
                inst = next(&argc, &argv);
                auxfile = sv(inst); 
            } else if (argc == 0) {
                inputfile = sv(inst);
            } else {
                fprintf(stderr, "Unknown option: %s\n", inst);
                exit(1);
            }
        }

        mount_tex_file(inputfile, auxfile);
        if (set_pdf) 
            mount_pdf(inputfile);

#if 0

#endif
        return 0;
}

void mount_pdf(String_View input) {
    String texfile;
    String_View texfile_view = sv_from_string(&texfile);
    
    str_replace(&texfile, input, sv(".md"), sv(".tex"));
    String_View path = sv_get_char_from_right(&texfile_view, PATH_SEP);
    
    if (sv_eq(path, texfile_view)) {
        execl(TEX_COMPILER, TEX_COMPILER, str_cstr(&texfile), (char *) NULL);
    } else {
        const char *cstr_path = sv_cstr(path);
        execl(TEX_COMPILER, TEX_COMPILER, "-output-directory", cstr_path, str_cstr(&texfile), (char *) NULL);
        free((char *)cstr_path);
    }
}

void mount_tex_file(String_View mdfile, String_View auxfile) {

    String_View md_contents = readfile(mdfile);
    yml_parse(&yml, &md_contents);
    
    FILE *out  = tex_openfile(mdfile);
    tex_init(&yml, auxfile, out);
    tex_parse(sv_trim_left(md_contents), out);
    
    fclose(out);
}

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

void usage(FILE *stream) {
    // TODO: Write complete documentation. 
    const char *u = 
    "LTK - Markdown LaTeX converter\n"
    "Usage:\n"
    "\tltk [options] [file]\n"
    "Options:\n"
    "\t-g           Generates sample md file\n"
    "\t-a           Appends auxiliary tex file in preamble\n"
    "\t--pdf        Generates tex and compiles with pdflatex pdf file\n";
    fprintf(stream, "%s\n", u);
    exit(0);
}
