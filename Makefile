COMMONFLAGS = -Wall -Werror -Wextra -Wno-unused-function -pedantic -I. -std=c99
DEBUG = -ggdb
CC = gcc

run: ltk.c
	$(CC) -o ltk ltk.c
	./ltk

ltk: ltk.c
	$(CC) $(COMMONFLAGS) -o ltk ltk.c
	./ltk

debug: run
	$(CC) $(DEBUG) -o ltk ltk.c
	gdb ./ltk

clean:
	rm ltk tt


.PHONY: clean, run
