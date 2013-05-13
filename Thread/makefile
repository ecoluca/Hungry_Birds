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
# This is my manual version of makefile for Hungry Birds Thread
#  
#    

CC = gcc
LD = gcc
CFLAGS = -Wall -lpthread -c
LFLAGS = -Wall -lpthread -o
GDB = -g

all: hungry_birds_thread

hungry_birds_thread :  hungry_birds_thread.o
	$(LD) $(LFLAGS)  hungry_birds_thread  hungry_birds_thread.o

hungry_birds_thread.o : hungry_birds_thread.c
	$(CC) $(CFLAGS) hungry_birds_thread.c

clean : 
	rm -rf *.o
	rm -rf hungry_birds_thread

cleanall:
	rm -rf *.o
	rm -rf *~
	rm -fr *#
	rm -rf hungry_birds_thread 
