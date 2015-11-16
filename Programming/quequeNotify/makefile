# Makefile for ReceiverTransmitter messages in terminal

CC=gcc
CFLAGS=-c -Wall -Wextra -g
PNAME=childn

main: childn.o
	$(CC) -o $(PNAME) -g childn.o
	rm -f childn.o

RTterm.o: childn.c
	$(CC) $(CFLAGS) childn.c

clean:	
	rm *.o $(PNAME)

