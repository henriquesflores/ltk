COMMONFLAGS = -Wall -Werror -Wextra -Wno-unused-function -pedantic -I. -std=c99
DEBUG = -ggdb
CC = gcc

basic: src/ltk.c
	$(CC) -o src/ltk src/ltk.c

debug: basic 
	$(CC) $(DEBUG) -o src/ltk src/ltk.c
	gdb ./src/ltk

ltk: src/ltk.c
	$(CC) $(COMMONFLAGS) -o src/ltk src/ltk.c

clean:
	rm tests/complete.aux
	rm tests/complete.log
	rm tests/complete.tex
	rm tests/complete.out
	rm tests/complete.toc
	rm tests/complete.pdf

.PHONY: basic, debug, clean
