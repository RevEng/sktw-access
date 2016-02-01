# vim: set ts=4 sw=4 ai :

LIBDIR=./lib
INCLUDEDIR=./include

all: test

test: test.c
	gcc -I$INCLUDEDIR -L$LIBDIR -lsoc test.c -o test

.PHONY: all

