CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99 -O2 -march=native
RELEASE_FLAGS=-DNDEBUG -s
DEBUG_FLAGS=-g

.PHONY: all debug clean


all: solution

solution: solution.o tree.o list.o
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) solution.o tree.o list.o -o solution

solution.o: solution.c
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c solution.c 

tree.o: tree.c
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c tree.c

list.o: list.c
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c list.c


debug: solution.dbg

solution.dbg: solution.dbg.o tree.dbg.o list.dbg.o
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) solution.dbg.o tree.dbg.o list.dbg.o -o solution.dbg

solution.dbg.o: solution.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c solution.c -o solution.dbg.o

tree.dbg.o: tree.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c tree.c -o tree.dbg.o

list.dbg.o: list.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c list.c -o list.dbg.o


clean:
	-rm -f *.o solution solution.dbg massif.out.*
