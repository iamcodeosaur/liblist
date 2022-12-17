liblist.a: liblist.o
	ar -rc liblist.a liblist.o

liblist.o: liblist.c list.h
	clang -Wall -Werror -Wextra --pedantic-errors --std=c99 -c -o liblist.o liblist.c
