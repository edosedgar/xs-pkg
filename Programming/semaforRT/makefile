# Makefile for ReceiverTransmitter messages in terminal

CC=gcc
CFLAGS=-c -Wall -g
PNAME=semrt

main: semaforRT.o
	$(CC) -o $(PNAME) -g semaforRT.o
	rm -f semaforRT.o

RTterm.o: semaforRT.c
	$(CC) $(CFLAGS) semaforRT.c

clean:	
	rm *.o $(PNAME)

