#ifndef TEMPLATE_H_
#define TEMPLATE_H_

static char *template_doc = 
"---\n"
"title: This is a good title\n"
"author: I'm the author\n"
"letter: 12\n"
"type: article\n"
"date: true\n"
"toc: true\n"
"any other field written in here is ignored\n"
"---\n"
"\n"
"## Introduction\n"
"\n"
"This sentence has **bold parsing** in final latex.\n"
"For equations:\n"
"\n"
"$$ \n"
"E = mc^{2}\n"
"$$\n"
"\n"
"## Custom Section\n"
"\n"
"Given *this custom section*, we can have\n"
"\n"
"### Subsection\n"
"\n"
"$$ \n"
"a^{2} + b^{2} = c^{2}\n"
"$$\n";

void dump_template(FILE *stream) {
    const char *tempfile = "template.md";
    
    FILE *f = fopen(tempfile, "w");
    if (!f) {
        fprintf(stream, "Cannot generate: %s\n", tempfile);
        exit(1);
    }
    fputs(template_doc, f);
    fclose(f);

    fprintf(stream, "Template generated at %s\n",tempfile);
    exit(0);
}

#endif // TEMPLATE_H_
