COMPILER=clang
FLAGS= -Wall -g -Wextra -std=c99
FILES= labyrinth.c main.c

all:
	$(COMPILER) $(FLAGS) $(FILES)

clean::
	rm -f *~ *.o
