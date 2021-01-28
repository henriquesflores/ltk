COMMONFLAGS = -Wall -Werror -Wextra -Wno-unused-function -pedantic -I. -std=c99
DEBUG = -ggdb
CC = gcc

ltk: ltk.c
	$(CC) $(CFLAGS) -o ltk ltk.c
	./ltk

run: ltk.c
	$(CC) -o ltk ltk.c
	./ltk

debug: ltk
	$(CC) $(DEBUG) -o ltk ltk.c
	gdb ./ltk

tt: tt.c
	$(CC) -o tt tt.c
	./tt

clean:
	rm ltk tt


.PHONY: clean, run
