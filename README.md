# LTK 
Markdown LaTeX converter written in C.

**This version does not support links**

# Dependencies 
LTK uses pdflatex for pdf compilation. It looks in the `/usr/bin` folder.

By default, ltk introduces a collection of packages which are unavoidable.
The complete list of packages can be found in 
[tex_packages](https://github.com/henriquesflores/ltk/blob/main/src/tex.h).

# Usage

A custom md file can be generated with

``` console 
ltk -g
```

It exemplifies the usage of the markdown syntax.
The tex file can be obtained with 

``` console
ltk text.md 
Generating text.tex
```

or with option `--pdf`

``` console
ltk --pdf text.md
```

to run pdflatex and output *test.pdf*.

To append Latex custom commands, definitions and packages,
one writes them in an auxiliary file, for example `aux.tex`,
and ltk appends its content in the preamble:

``` console
ltk --pdf -a aux.tex text.md
```
