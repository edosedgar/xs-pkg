#Makefile for Logic.asm

Logic: Logic.o
	ld -o Logic Logic.o

Logic.o:  Startup.asm
	nasm -f elf64 -o Logic.o Startup.asm 
