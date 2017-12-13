CFLAGS = -D_REENTRANT -Wall -pedantic -Isrc -Iincludes -g3 -O3 -std=c11
LDLIBS = -lpthread -lthread_pool -lqsbr -Llib

TARGETS = tests/test liblinked_list.so liblinked_list.a

all: $(TARGETS)

tests/test: tests/test.o src/linked_list.o
src/linked_list.o: src/linked_list.c \
		   includes/linked_list.h \
		   includes/gc.h \
		   includes/qsbr.h

tests/test.o: tests/test.c \
	      includes/linked_list.h \
	      includes/thread_pool.h

shared: liblinked_list.so
static: liblinked_list.a

liblinked_list.so: src/linked_list.c includes/linked_list.h
	$(CC) -shared -fPIC ${CFLAGS} -o $@ $< ${LDLIBS}

src/liblinked_list.o: src/linked_list.c includes/linked_list.h
	$(CC) -c -fPIC ${CFLAGS} -o $@ $<

liblinked_list.a: src/liblinked_list.o
	ar rcs $@ $^

clean:
	rm -f $(TARGETS) *~ */*~ */*.o

test: $(TARGETS)
	./tests/test
