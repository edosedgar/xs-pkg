# Makefile for ReceiverTransmitter messages in terminal

CC=gcc
CFLAGS=-c -Wall -g
PNAME=sigrt

main: signalRT.o
	$(CC) -o $(PNAME) -g signalRT.o
	rm -f signalRT.o

RTterm.o: signalRT.c
	$(CC) $(CFLAGS) signalRT.c

clean:	
	rm *.o $(PNAME)

