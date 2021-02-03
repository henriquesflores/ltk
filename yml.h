#ifndef YML_H
#define YML_H

#include "ltk.h"

enum Doctype {
    NOTE = 0,
    ARTICLE,
};

typedef struct Yml {
    bool date;
    bool toc;
    enum Doctype type;
    String_View  letter;
    String_View  title;
    String_View  author;
} Yml;

Yml *yml_date(Yml *y, String_View file) {
    String_View date = sv_take_between(&file, sv("date:"), sv("\n"));

    if (sv_eq(date, file)) 
        return y;
    else if (sv_eq(date, sv("true")))
        y->date = true;
    else
        y->date = false;

    return y;
}

Yml *yml_toc(Yml *y, String_View file) {
    String_View tc = sv_take_between(&file, sv("toc:"), sv("\n"));
    
    if (sv_eq(tc, file))
        return y;
    else if (sv_eq(tc, sv("true")))
        y->toc = true;
    else
        y->toc = false;


    return y;
}

Yml *yml_letter(Yml *y, String_View file) {
    String_View ltr = sv_take_between(&file, sv("letter:"), sv("\n"));

    if (sv_eq(ltr, file)) 
        return y;
    else
        y->letter = ltr; 

    return y;
}

Yml *yml_type(Yml *y, String_View file) {
    String_View type = sv_take_between(&file, sv("type:"), sv("\n"));

    if (sv_eq(type, file))
        return y;
    else if (sv_eq(type, sv("note"))) 
        y->type = NOTE;
    else if (sv_eq(type, sv("article")))
        y->type = ARTICLE;

    return y;
}

Yml *yml_title(Yml *y, String_View file) {
    String_View ttl = sv_take_between(&file, sv("title:"), sv("\n"));

    if (sv_eq(ttl, file)) 
        return y;
    else 
        y->title = ttl;

    return y;
}

Yml *yml_author(Yml *y, String_View file) {
    String_View atr = sv_take_between(&file, sv("author:"), sv("\n"));

    if (sv_eq(atr, file))
        return y;
    else 
        y->author = atr;

    return y;
}

String_View yml_extract(String_View *file) { 
    String_View prev = sv_chop_str(file, sv("---"));
    if (sv_eq(prev, *file)) 
        return (String_View) {NULL, 0};

    String_View yml_env = sv_chop_str(file, sv("---"));
    if (sv_eq(yml_env, *file)) 
        return (String_View) {NULL, 0};
    else
        return yml_env;
}

Yml *yml_parse(Yml *y, String_View *file) {
    String_View yml_env = yml_extract(file);
    
    if (sv_isempty(yml_env))
        return y;

    yml_date(y, yml_env);
    yml_toc(y, yml_env);
    yml_letter(y, yml_env);
    yml_type(y, yml_env);
    yml_title(y, yml_env);
    yml_author(y, yml_env);
}

#endif // YML_H
