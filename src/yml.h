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

Yml *yml_toc(Yml *y, String_View file) {
    String_View choped = sv_chop_str(&file, sv("toc:"));
    
    if (sv_eq(choped, file))
        return y;

    String_View has_toc = sv_trim(sv_getline(&file));
    if (sv_eq(has_toc, sv("true")))
            y->toc = true;
    else
            y->toc = false;


    return y;
}

Yml *yml_letter(Yml *y, String_View file) {
    String_View choped = sv_chop_str(&file, sv("letter:"));

    if (sv_eq(choped, file)) 
        return y;

    y->letter = sv_trim(sv_getline(&file));
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
    yml_toc(y, yml_env);
    yml_letter(y, yml_env);
    yml_type(y, yml_env);
    yml_title(y, yml_env);
    yml_author(y, yml_env);
}

#endif // YML_H
