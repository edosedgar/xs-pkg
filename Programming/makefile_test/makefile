CFLAGS:= -Wall -Werror -O2 -MD

all: a.out

a.out: 1.o 2.o
	gcc -o $@ $^

-include *.d

clean:
	rm -rf *.o a.out

.PHONY: all clean
