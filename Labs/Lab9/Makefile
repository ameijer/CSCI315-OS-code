
CC = gcc -I ./include
CFLAGS = -std=gnu99 -Wall -g #-DDEBUG

INC = ./include
SRC = ./src
OBJ = ./obj
DOC = ./doc
BIN = ./bin

vpath %.h ./include
vpath %.c ./src

EXECS = readlinetest dlisttest allocator memory-test 

all: $(EXECS)

doc:
	doxygen

$(OBJ)/dnode.o: dnode.h dnode.c
	$(CC) $(CFLAGS) -c $(SRC)/dnode.c -o $(OBJ)/dnode.o

$(OBJ)/dlist.o: dlist.h dlist.c dnode.h
	$(CC) $(CFLAGS) -c $(SRC)/dlist.c -o $(OBJ)/dlist.o

dlisttest: $(SRC)/dlisttest.c $(OBJ)/dnode.o $(OBJ)/dlist.o
	$(CC) $(CFLAGS) $(OBJ)/dnode.o $(OBJ)/dlist.o $(SRC)/dlisttest.c -o $(BIN)/dlisttest

$(OBJ)/readline.o: $(INC)/readline.h $(SRC)/readline.c
	$(CC) $(CFLAGS) -c $(SRC)/readline.c -o $(OBJ)/readline.o

readlinetest: $(SRC)/readlinetest.c $(OBJ)/readline.o
	$(CC) $(CFLAGS) $(OBJ)/readline.o $(SRC)/readlinetest.c -o $(BIN)/readlinetest

allocator: allocator.c allocator.h dlist.h dlist.c dnode.h dnode.h dnode.c
	$(CC) $(CFLAGS) $(SRC)/allocator.c $(OBJ)/dnode.o $(OBJ)/dlist.o -o $(OBJ)/allocator.o -c

memory-test:
	$(CC) $(CFLAGS) $(SRC)/memory-test-1.c $(OBJ)/allocator.o $(OBJ)/dnode.o $(OBJ)/dlist.o $(INC)/allocator.h -o $(BIN)/memory-test-1

.PHONY: clean
clean:
	/bin/rm -rf $(BIN)/* $(OBJ)/* $(DOC)/* core* *~

