# Makefile for ReceiverTransmitter messages in terminal

CC=gcc
CFLAGS=-c -Wall -g
PNAME=rtterm

main: RTterm.o
	$(CC) -o $(PNAME) -g RTterm.o
	rm -f RTterm.o

RTterm.o: RTterm.c
	$(CC) $(CFLAGS) RTterm.c

clean:	
	rm *.o $(PNAME)

