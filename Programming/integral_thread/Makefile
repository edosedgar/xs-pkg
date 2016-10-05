CFLAGS:=-Wall -pthread -Wextra -g3 -O0 -std=gnu99
LDLIBS:=-lm 
PROJECT_NAME:=integral
SOURCE:=main.c

.PHONY: clean

$(PROJECT_NAME): $(SOURCE)
	$(CC) $(CFLAGS) $(LDLIBS)-o $@ $<

clean:
	rm -rf *.o $(PROJECT_NAME)




