# A Meijer
#Makefile

#
# CSCI 315 Operating Systems Design
# Author: L. Felipe Perrone, modified by alexander Meijer
# Date: 2011-04-21
# Copyright (c) 2011 Bucknell University
#
# Permission is hereby granted, free of charge, to any individual or
# institution obtaining a copy of this software and associated
# documentation files (the "Software"), to use, copy, modify, and
# distribute without restriction, provided that this copyright and
# permission notice is maintained, intact, in all copies and supporting
# documentation.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL BUCKNELL UNIVERSITY BE LIABLE FOR ANY CLAIM, DAMAGES
# OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# compiler
CC = gcc

CFLAGS = -g -Wall

EXE = file_stat read_dir hexdump fdump traverse

all: $(EXE)

file_stat: file_stat.o
	$(CC) $(CFLAGS) -o file_stat file_stat.o

file_stat.o: file_stat.c
	$(CC) -c $(CFLAGS) file_stat.c

read_dir: read_dir.o
	$(CC) -o read_dir read_dir.o

read_dir.o: read_dir.c
	$(CC) -c $(CFLAGS) read_dir.c

hexdump:
	$(CC) $(CFLAGS) -c hexdump.c -o hexdump.o

fdump:
	$(CC) $(CFLAGS) hexdump.o fdump.c -o fdump  hexdump.h

traverse:
	$(CC) $(CFLAGS) traverse.c read_dir.h -o traverse


clean:
	/bin/rm -f *~ *.o core $(EXE)
