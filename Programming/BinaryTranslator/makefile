# Makefile for binary translator ( STM8 )

BinaryTranslator: Translator.o ARM.o main.o
	g++ ARM.o Translator.o   main.o -o BinaryTranslator

main.o: main.cpp
	g++ -c main.cpp

Translator.o: Translator.cpp
	g++ -c Translator.cpp

ARM.o: ARM.cpp
	g++ -c ARM.cpp

clean: 
	rm ./*.o
