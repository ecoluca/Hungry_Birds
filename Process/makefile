#
# makefile - Personal Version of Makefile
#
# Copyright (C) 2013 Luca Contri
# Author: Luca Contri <LucaContri86@gmail.com>
#
#
# Command Compile: 
#   make -f MY_Makefile
#   
# This is my manual version of makefile for Hungry Birds Process
#  
#

CC = gcc
LD = gcc
CFLAGS = -Wall -lpthread -lrt -c
LFLAGS = -Wall -lpthread -lrt -o
GDB = -g

all: hungry_birds_process child_process parent_process

hungry_birds_process:  hungry_birds_process.o
	$(LD) $(LFLAGS) hungry_birds_process  hungry_birds_process.o

child_process: child_process.o
	$(LD) $(LFLAGS) child_process child_process.o

parent_process: parent_process.o
	$(LD) $(LFLAGS) parent_process parent_process.o

hungry_birds_process.o: hungry_birds_process.c
	$(CC) $(CFLAGS) hungry_birds_process.c

child_process.o: child_process.c
	$(CC) $(CFLAGS) child_process.c

parent_process.o: parent_process.c
	$(CC) $(CFLAGS) parent_process.c

clean : 
	rm -rf *.o
	rm -rf 	hungry_birds_process \
		child_process \
		parent_process

cleanall:
	rm -rf *.o
	rm -rf *~
	rm -fr *#
	rm -rf 	hungry_birds_process \
		child_process \
		parent_process


