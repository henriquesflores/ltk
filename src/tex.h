#ifndef TEX_H_
#define TEX_H_

#include "ltk.h"
#include "yml.h"

typedef struct Cmd {
    String_View md_open;
    String_View md_close;
    String_View tex_open;
    String_View tex_close;
} Cmd;

static char *tex_documentclass;
static char *tex_begindoc; 
static char *tex_enddoc; 
static char *tex_packages;

static char *tex_title; 
static char *tex_maketitle; 
static char *tex_toc; 
static char *tex_author; 
static char *tex_date; 
static char *tex_section;
static char *tex_subsection;
// static char *tex_chapter;  WILL BE IMPLEMENTED

static char *tex_equation_o;
static char *tex_equation_c;
static char *tex_itemize_o;
static char *tex_itemize_c;
static char *tex_item;

static char *tex_bf;
static char *tex_it;
static char *tex_un;
static char *tex_close_env;

static char *md_section;
static char *md_subsection;
static char *md_bf;
static char *md_it;
static char *md_un;
static char *md_item;
static char *md_equation;

Cmd *cmd_section(Cmd *cmd) {
    cmd->md_open   = sv(md_section);
    cmd->md_close  = sv("\n");
    cmd->tex_open  = sv(tex_section);
    cmd->tex_close = sv(tex_close_env); 

    return cmd;
}

Cmd *cmd_subsection(Cmd *cmd) {
    cmd->md_open   = sv(md_subsection);
    cmd->md_close  = sv("\n");
    cmd->tex_open  = sv(tex_subsection);
    cmd->tex_close = sv(tex_close_env); 

    return cmd;
}

Cmd *cmd_bf(Cmd *cmd) {
    cmd->md_open   = sv(md_bf);
    cmd->md_close  = sv(md_bf);
    cmd->tex_open  = sv(tex_bf);
    cmd->tex_close = sv(tex_close_env);

    return cmd;
}

Cmd *cmd_it(Cmd *cmd) {
    cmd->md_open   = sv(md_it);
    cmd->md_close  = sv(md_it);
    cmd->tex_open  = sv(tex_it);
    cmd->tex_close = sv(tex_close_env);

    return cmd;
}

Cmd *cmd_un(Cmd *cmd) {
    cmd->md_open   = sv(md_un);
    cmd->md_close  = sv(md_un);
    cmd->tex_open  = sv(tex_un);
    cmd->tex_close = sv(tex_close_env);

    return cmd;
}

Cmd *cmd_itemize(Cmd *cmd) {
    cmd->md_open   = (String_View) {NULL, 0};
    cmd->md_close  = (String_View) {NULL, 0};
    cmd->tex_open  = sv(tex_itemize_o);
    cmd->tex_close = sv(tex_itemize_c);

    return cmd;
}

Cmd *cmd_item(Cmd *cmd) {
    cmd->md_open   = sv(md_item);
    cmd->md_close  = sv("\n");
    cmd->tex_open  = sv(tex_item);
    cmd->tex_close = (String_View) {NULL, 0};

    return cmd;
}

Cmd *cmd_equation(Cmd *cmd) {
    cmd->md_open   = sv(md_equation);
    cmd->md_close  = sv(md_equation);
    cmd->tex_open  = sv(tex_equation_o);
    cmd->tex_close = sv(tex_equation_c);

    return cmd;
}

FILE *tex_openfile(String_View md_file) {

        String texfile;
        str_replace(&texfile, md_file, sv(".md"), sv(".tex"));
        printf("Generating file:\n\t"SVFMT"\n", SVARG(texfile));
       
        FILE *stream = fopen(str_cstr(&texfile), "w");
        return stream;
}

FILE *tex_init(
        Yml *y, 
        String_View aux,
        FILE *tex_file
) {

    assert(tex_file);

    String main_buffer; 
    String helper_buffer; 

    str_append(&main_buffer, y->letter, sv("pt"));
    str_replace(&helper_buffer, sv(tex_documentclass), sv("x"), sv_from_string(&main_buffer));
    str_copy(&main_buffer, &helper_buffer);
    
    String_View auxcontents = {NULL, 0};    
    if (!sv_isempty(aux))
        auxcontents = readfile(aux);
   
    // TODO: doctype is entangled with document format. 
    // This does not need to be the case.
    String_View doc_type;
    switch (y->type) {
        case NOTE: {
            doc_type = sv("article");
            str_replace(&main_buffer, sv_from_string(&helper_buffer), sv("y"), doc_type);
            fprintf(tex_file, SVFMT"\n", SVARG(main_buffer));
            fprintf(tex_file, "%s\n\n", tex_packages);
            if (auxcontents.str) {
                fprintf(tex_file, "%% Appending from: " SVFMT "\n", SVARG(aux));
                fprintf(tex_file, SVFMT, SVARG(auxcontents));
                fprintf(tex_file, "%% Appending ends\n\n");
            }
        } break;

        case ARTICLE: { 
            doc_type = sv("article");
            str_replace(&main_buffer, sv_from_string(&helper_buffer), sv("y"), doc_type);
            fprintf(tex_file, SVFMT"\n", SVARG(main_buffer));
            fprintf(tex_file, "%s\n\n", tex_packages);
            if (auxcontents.str) {
                fprintf(tex_file, "%% Appending from: " SVFMT "\n", SVARG(aux));
                fprintf(tex_file, SVFMT, SVARG(auxcontents));
                fprintf(tex_file, "%% Appending ends\n\n");
            }
        } break; 

        default: {
            assert(0 && "Type not implemented!");
        } break;
    }

    free((char *)auxcontents.str);

    str_replace(&main_buffer, sv(tex_title),  sv("x"), y->title);
    str_replace(&helper_buffer, sv(tex_author), sv("x"), y->author);
    fprintf(tex_file, SVFMT"\n", SVARG(main_buffer));
    fprintf(tex_file, SVFMT"\n", SVARG(helper_buffer));

    if (y->date) {
        fprintf(tex_file, "%s\n\n", tex_date);
    }
    
    fprintf(tex_file, "%s\n", tex_begindoc);

    switch (y->type) {
        case NOTE: {
            fprintf(tex_file, "\\begin{center}\n");
            fprintf(tex_file, "\\Large{\\textbf{"SVFMT"}}\n\\\\\n", SVARG(y->title));
            fprintf(tex_file, SVFMT"\n", SVARG(y->author));
            fprintf(tex_file, "\\end{center}\n");
            fputs("\n", tex_file);
        } break;

        case ARTICLE: {
            fprintf(tex_file, "%s\n\n", tex_maketitle); 
        } break; 

        default: {
            assert(0 && "Type not implemented!");
        } break;

    }
   
    if (y->toc) {
        fprintf(tex_file, "%s\n", tex_toc);
    }

    return tex_file;
}

String *parse_markdown(
        String *buffer,
        String_View str,
        Cmd *cmd
) {
    size_t nm = 0;
    if (cmd->md_open.len == 1) 
        nm = sv_count_char(str, *cmd->md_open.str); 
    else 
        nm = sv_count_str(str, cmd->md_open);

    if (!nm) {
        str_copysv(buffer, str);
        return buffer;
    }
    
    String holder;
    str_copysv(&holder, str);
    while (nm) {
        if (nm-- % 2 == 0) 
            str_replace(buffer, sv_from_string(&holder), cmd->md_open, cmd->tex_open);
        else 
            str_replace(buffer, sv_from_string(&holder), cmd->md_close, cmd->tex_close);
    
        str_copy(&holder, buffer); 
    } 
    
    return buffer;
}

String *parse_headers(
        String *buffer1,
        String *buffer2,
        String_View str,
        Cmd *cmd
) {

    String h;  
    str_copysv(buffer1, str);

    size_t count = sv_count_str(str, cmd->md_open);
    while (count--) {
        String_View field = sv_take_between(&str, cmd->md_open, cmd->md_close);
        str_replace(&h, cmd->tex_open, sv("x"), field);
        str_replace_between(buffer2, sv_from_string(buffer1), cmd->md_open, cmd->md_close, sv_from_string(&h));
        str_copy(buffer1, buffer2);
    }

    return buffer1;
}

void tex_parse(String_View file, FILE *stream) {

    Cmd commands = {
        .md_open   = (String_View) {NULL, 0},
        .md_close  = (String_View) {NULL, 0},
        .tex_open  = (String_View) {NULL, 0},
        .tex_close = (String_View) {NULL, 0},
    };

    Cmd *cmd = &commands; 

    String global;
    String helper;
    str_copysv(&global, file);
    str_copysv(&helper, file);

    cmd_subsection(cmd);
    parse_headers(&global, &helper, sv_from_string(&helper), cmd);
    str_copy(&global, &helper);
 
    cmd_section(cmd);
    parse_headers(&global, &helper, sv_from_string(&global), cmd);
    str_copy(&global, &helper);
   
    cmd_equation(cmd);
    parse_markdown(&helper, sv_from_string(&global), cmd);
    str_copy(&global, &helper);

    cmd_bf(cmd);
    parse_markdown(&helper, sv_from_string(&global), cmd);
    str_copy(&global, &helper);

    cmd_it(cmd);
    parse_markdown(&helper, sv_from_string(&global), cmd);
    str_copy(&global, &helper);

    cmd_un(cmd);
    parse_markdown(&helper, sv_from_string(&global), cmd);
    str_copy(&global, &helper);
    
    fprintf(stream, SVFMT, SVARG(global)); 
    fprintf(stream, "%s", tex_enddoc);
} 

static char *md_section    = "## ";
static char *md_subsection = "### ";
static char *md_bf         = "**";
static char *md_it         = "*";
static char *md_un         = "@";
static char *md_item       = " -";
static char *md_equation   = "$$";

static char *tex_documentclass = "\\documentclass[x]{y}\n";
static char *tex_begindoc      = "\\begin{document}\n";
static char *tex_enddoc        = "\\end{document}\n";

static char *tex_title         = "\\title{x}\n";
static char *tex_maketitle     = "\\maketitle\n";
static char *tex_toc           = "\\tableofcontents\n";
static char *tex_author        = "\\author{x}\n";
static char *tex_date          = "\\date{\\today}\n";
static char *tex_section       = "\\section{x}\n";
static char *tex_subsection    = "\\subsection{x}\n";
//static char *tex_chapter       = "\\chapter{x}\n";

static char *tex_equation_o    = "\\begin{align}";
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

#endif // TEX_H_
