#ifndef TEX_H
#define TEX_H

#include <stdio.h>
#include "ltk.h"
#include "yml.h"

// TEX main commands declarations.
// Definitions at the end of file.
static char *tex_documentclass;
static char *tex_begindoc; 
static char *tex_enddoc; 

static char *tex_title; 
static char *tex_maketitle; 
static char *tex_toc; 
static char *tex_author; 
static char *tex_date; 
static char *tex_section;
static char *tex_subsection;
static char *tex_chapter; 

static char *tex_equation_o;
static char *tex_equation_c;
static char *tex_itemize_o;
static char *tex_itemize_c;
static char *tex_item;

static char *tex_bf;
static char *tex_it;
static char *tex_un;
static char *tex_close_env;

static char *tex_packages;

void tex_init(Yml *y, FILE *tex_file) {
    char b1[KILO];
    String_View buffer1 = {b1, KILO};

    char b2[KILO];
    String_View buffer2 = {b2, KILO};

    sv_append_in_buffer(&buffer1, y->letter, sv("pt"));
    sv_replace_in_buffer(&buffer2, sv(tex_documentclass), sv("x"), buffer1);
   
    String_View doc_type;
    // TODO: doctype is entangled with document format. 
    // This does not need to be the case.
    switch (y->type) {
        case NOTE: {
            doc_type = sv("article");
        } break;

        case ARTICLE: {
            doc_type = sv("article");
        } break; 

        default: {
            assert(0 && "Type not implemented!");
        } break;
    }

    sv_copy(&buffer1, &buffer2);
    sv_replace_in_buffer(&buffer2, buffer1, sv("y"), doc_type);
    fprintf(tex_file, SVFMT"\n", SVARG(buffer2));
    fputs(tex_packages, tex_file);
    fputs("\n\n", tex_file);
    sv_replace_in_buffer(&buffer1, sv(tex_title),  sv("x"), y->title);
    sv_replace_in_buffer(&buffer2, sv(tex_author), sv("x"), y->author);
    fprintf(tex_file, SVFMT"\n", SVARG(buffer1));
    fprintf(tex_file, SVFMT"\n", SVARG(buffer2));

    if (y->date) {
        fputs("\\date{}\n", tex_file);
        fputs("\n", tex_file);
    }

    fputs(tex_begindoc, tex_file);
    fputs("\n", tex_file);

    switch (y->type) {
        case NOTE: {
            fprintf(tex_file, "\\begin{center}\n");
            fprintf(tex_file, "\\Large{\\textbf{"SVFMT"}}\n", SVARG(y->title));
            fprintf(tex_file, SVFMT"\n", SVARG(y->author));
            fprintf(tex_file, "\\end{center}\n");
            fputs("\n", tex_file);
        } break;

        case ARTICLE: {
            fputs(tex_maketitle, tex_file); 
            fputs("\n", tex_file);
        } break; 

        default: {
            assert(0 && "Type not implemented!");
        } break;

    }
   
    if (y->toc) 
        fputs(tex_toc, tex_file);


}

static char *tex_documentclass = "\\documentclass[x]{y}\n";
static char *tex_begindoc      = "\\begin{document}\n";
static char *tex_enddoc        = "\\end{document}\n";

static char *tex_title         = "\\title{x}\n";
static char *tex_maketitle     = "\\maketitle\n";
static char *tex_toc           = "\\tableofcontents\n";
static char *tex_author        = "\\author{x}\n";
static char *tex_date          = "\\date{}\n";
static char *tex_section       = "\\section{x}\n";
static char *tex_subsection    = "\\subsection{x}\n";
static char *tex_chapter       = "\\chapter{x}\n";

static char *tex_equation_o    = "\\begin{align}\n";
static char *tex_equation_c    = "\\end{align}\n";
static char *tex_itemize_o     = "\\begin{itemize}\n";
static char *tex_itemize_c     = "\\end{itemize}\n";
static char *tex_item          = "\\item";

static char *tex_bf            = "\\textbf{";
static char *tex_it            = "\\textit{";
static char *tex_un            = "\\underline{";
static char *tex_close_env     = "}";

static char *tex_packages      = 
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
"\\usepackage{float}\n"
"\n"
"\\numberwithin{equation}{section}\n"
"\n"
"\\newtheoremstyle{henrique}        % Name\n"
"  { }                             % Space above\n"
"  {}                              % Space below\n"
"  {\\normalfont}                   % Body font\n"
"  {}                              % Indent amount\n"
"  {\\bfseries}                     % Theorem head font\n"
"  {.}                             % Punctuation after theorem head\n"
"  { }                             % Space after theorem head, ' ', or \\newline\n"
"  {}                              % Theorem head spec (can be left empty, meaning `normal')\n"
"\\theoremstyle{henrique}\n"
"\\newtheorem{teo}{Theorem}[section]\n"
"\\newtheorem{defi}{Definition}[section]\n"
"\\newtheorem{lema}{Lemma}[section]\n"
"\\newtheorem{corol}{Corollary}[section]\n"
"\\newtheorem{prop}{Proposition}[section]\n"
"\n"
"\n"
"\\hypersetup{colorlinks=false,\n"
"            urlbordercolor  = {1 0 0},\n"
"            linkbordercolor = {0 0 1},\n"
"            citebordercolor = {0 0 1} }\n";

#endif // TEX_H
