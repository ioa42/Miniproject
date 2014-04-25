CC=gcc
CFLAGS= -Wall -Wextra -std=c99
SRC= malloc.c

all:
	$(CC) $(CFLAGS) -o main $(SRC) 

debug: CFLAGS += -g
debug: CC=clang
debug: all

release: CFLAGS += -O2
release: all

lib::
	clang $(CFLAGS) -fPIC -g -c -o malloc.o $(SRC)
	clang -shared -o libmalloc.so malloc.o

clean:
	    rm -f *~ *.o

