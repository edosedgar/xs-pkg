CFLAGS:=-Wall --coverage -Wextra -Werror -MD -g -I$(PWD)/include/
.PHONY: all clean

all: hash

hash: main.c hash_table.o
	$(CC) --coverage -I$(PWD)/include -g -o $@  $^

-include *.d

clean: 
	rm -rf *.o ./include/*.o hash *~ ./include/*~ ./doc/*~ ./*.swp ./.*.swp

doxygen:
	doxygen $(PWD)/doc/doxygen.conf
