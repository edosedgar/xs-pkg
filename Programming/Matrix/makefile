#Makefile for Matrix

Matrix: main.o
	g++ main.o -o Matrix
	
main.o: main.cpp
	g++ -std=c++11 -c main.cpp
	
Matrix.o: Matrix.cpp Matrix.h
	rm -f ./Matrix.o
	rm -f ./main.o
	g++ -std=c++11 -c Matrix.cpp
	
clean:
	rm ./*.o
