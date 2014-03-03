#
# Copyright (c) 2012 Bucknell University
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation;
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# Author: L. Felipe Perrone (perrone@bucknell.edu)
#

CC = gcc -I ./include
CFLAGS = -std=gnu99 -Wall -pthread -g #-DDEBUG

INC = ./include
SRC = ./src
OBJ = ./obj
DOC = ./doc
BIN = ./bin

vpath %.h ./include
vpath %.c ./src

EXECS =	solution adt-test

all: $(EXECS)

doc:
	doxygen


### BEGIN CHANGES BY GRADER ###
#$(OBJ)/circular-list.o: $(INC)/circular-list.h $(SRC)/circular-list.c 
#	$(CC) $(CFLAGS) -c $(SRC)/circular-list.c -o $(OBJ)/circular-list.o

#solution: $(OBJ)/circular-list.o
#	$(CC) $(CFLAGS) $(SRC)/solution.c $(OBJ)/circular-list.o -o $(BIN)/solution -lpthread

solution: $(INC)/circular-list.h $(SRC)/circular-list.c 
	$(CC) $(CFLAGS) $(SRC)/solution.c -DENABLE_BLOCKING $(SRC)/circular-list.c -o $(BIN)/solution -lpthread
### END CHANGES BY GRADER ###

adt-test: 
	$(CC) $(CFLAGS) $(SRC)/adt-test.c $(OBJ)/circular-list.o -o $(BIN)/adt-test

.PHONY: clean
clean:
	#/bin/rm -f $(OBJ) $(EXECS) core* *~
	/bin/rm -f ./obj/*
	/bin/rm -rf ./doc
	/bin/rm -f ./include/*~
	/bin/rm -f ./src/*~
	/bin/rm -f ./bin/*
