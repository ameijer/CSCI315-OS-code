# Makefile for CSCI 315 Operating Systems Design
# Author: Alexander Meijer
# Date: 2-14-2013
#
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
#

CC = gcc 
CFLAGS = -std=gnu99 -Wall -g #-DDEBUG 


vpath %.h ./include
vpath %.c ./src

EXECS = timer clock user

all: $(EXECS)


timer: timer.c
	$(CC) $(CFLAGS) timer.c -o timer 

clock: 
	$(CC) $(CFLAGS) clock.c -o clock

user: 
	$(CC) $(CFLAGS) user.c -o user

.PHONY: clean
clean:
	/bin/rm -rf *~ timer clock user
