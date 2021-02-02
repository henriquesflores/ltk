#ifndef TEX_H
#define TEX_H

#include <stdio.h>
#include "ltk.h"

enum Doctype {
    NOTE = 0,
    ARTICLE,
};

typedef struct Yml {
    bool date;
    uint8_t letter;
    enum Doctype type;
    String_View title;
    String_View author;
} Yml;

Yml *yml_date(Yml *y, String_View file);
Yml *yml_letter(Yml *y, String_View file);
Yml *yml_type(Yml *y, String_View file);
Yml *yml_title(Yml *y, String_View file);
Yml *yml_author(Yml *y, String_View file);
Yml *yml_parse(Yml *y, String_View file);
String_View yml_extract(String_View file);

Yml *yml_date(Yml *y, String_View file) {
    String_View choped = sv_chop_str(&file, sv("date:"));
    
    if (sv_eq(choped, file)) 
        return y;

    String_View has_date = sv_trim(sv_getline(&file));
    if (sv_eq(has_date, sv("true")))
        y->date = true;
    else
        y->date = false;

    return y;
}

Yml *yml_letter(Yml *y, String_View file) {
    String_View choped = sv_chop_str(&file, sv("letter:"));

    if (sv_eq(choped, file)) 
        return y;

    String_View has_letter = sv_trim(sv_getline(&file));
    sscanf(has_letter.str, "%hhd",  &y->letter);
    return y;
}

Yml *yml_type(Yml *y, String_View file) {
    String_View choped = sv_chop_str(&file, sv("type:"));

    if (sv_eq(choped, file))
        return y;

    String_View has_type = sv_trim(sv_getline(&file));
    if (sv_eq(has_type, sv("note"))) 
        y->type = NOTE;
    else if (sv_eq(has_type, sv("article")))
        y->type = ARTICLE;

    return y;
}

Yml *yml_title(Yml *y, String_View file) {
    String_View choped = sv_chop_str(&file, sv("title:"));

    if (sv_eq(choped, file)) 
        return y;

    String_View title = sv_trim(sv_getline(&file));
    y->title = title;
    return y;
}

Yml *yml_author(Yml *y, String_View file) {
    String_View choped = sv_chop_str(&file, sv("author:"));

    if (sv_eq(choped, file))
        return y;

    String_View author = sv_trim(sv_getline(&file));
    y->author = author;
    return y;
}

String_View yml_extract(String_View file) { 
    sv_chop_str(&file, sv("---"));
    sv_getline(&file);
    String_View yml_environ = sv_chop_str(&file, sv("---"));
    
    if (sv_eq(yml_environ, file)) 
        return (String_View) {NULL, 0};
    else
        return yml_environ;
}

Yml *yml_parse(Yml *y, String_View file) {
    String_View yml_env = yml_extract(file);
    
    if (sv_isempty(yml_env))
        return y;

    yml_date(y, yml_env);
    yml_letter(y, yml_env);
    yml_type(y, yml_env);
    yml_title(y, yml_env);
    yml_author(y, yml_env);
}

//void yml_dumptex(Yml *y, FILE *f) {}

static char *tex_documentclass = "\\documentclass[x]{y}";
static char *tex_begindoc      = "\\begin{document}\n";
static char *tex_enddoc        = "\\end{document}\n";
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
"            urlbordercolor={1 0 0},\n"
"            linkbordercolor={0 0 1},\n"
"            citebordercolor={0 0 1} }\n";

#endif // TEX_H
