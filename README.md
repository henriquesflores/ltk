# LTK 
Markdown LaTeX converter written in C.

**This version does not support links**

# Dependencies 
LTK uses pdflatex for pdf compilation. It looks in the `/usr/bin` folder.

By default, ltk introduces a collection of packages which are unavoidable.
The complete list of packages can be found in 
[https://github.com/henriquesflores/ltk/blob/main/src/tex.h](tex_packages).

# Usage

``` console
ltk text.md 
Generating text.tex
```

or with option `--pdf`

``` console
ltk --pdf text.md
```

to run pdflatex and output *test.pdf*
