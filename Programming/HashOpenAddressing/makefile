CFLAGS:=-Wall -Wextra -Werror -MD -I$(PWD)/include/
.PHONY: all clean

all: hash

hash: main.c hash_table.o
	$(CC) -I$(PWD)/include -o $@  $^

-include *.d

clean: 
	rm -rf *.o ./include/*.o hash *~ ./include/*~ ./doc/*~ ./*.swp ./.*.swp

doxygen:
	doxygen $(PWD)/doc/doxygen.conf
