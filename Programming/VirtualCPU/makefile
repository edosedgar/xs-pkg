#Virtual CPU makefile

VirtualCPU: MyStack.o VirtualCPU.o main.o
	g++ -o VirtualCPU MyStack.o VirtualCPU.o main.o

MyStack.o: MyStack.cpp MyStack.h
	g++ -c MyStack.cpp

VirtualCPU.o: VirtualCPU.cpp VirtualCPU.h
	g++ -c VirtualCPU.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm ./*.o

