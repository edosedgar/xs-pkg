# Makefile for proxy server

CC=gcc
CFLAGS=-c -Wall -g
PNAME=proxy

main: proxy_server.o
	$(CC) -o $(PNAME) proxy_server.o -lm
	rm -f proxy_server.o

proxy_server.o: proxy_server.c
	$(CC) $(CFLAGS)  proxy_server.c

clean:	
	rm *.o $(PNAME)

