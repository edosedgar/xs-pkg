# Makefile for Compiler

Compiler: Compiler.o main.o
	g++ Compiler.o main.o -o Compiler

Compiler.o: Compiler.cpp
	g++ -fpermissive -c Compiler.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm ./*.o
