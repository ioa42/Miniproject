# Makefile with basic concepts
 
# Define compiler and options
CC=clang
CFLAGS= -Wall -Wextra -std=c99
FILES = hash_table.c

all:
	$(CC) $(CFLAGS) $(FILES)

debug: CFLAGS += -d
debug: all
release: CFLAGS += -O3
release: all


clean:
	    rm -f *~ *.o
 
# END (do not delete)
