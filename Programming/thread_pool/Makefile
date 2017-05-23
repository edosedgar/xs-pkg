CFLAGS = -D_REENTRANT -Wall -pedantic -Isrc -g3 -O3
LDLIBS = -lpthread

TARGETS = tests/test libthread_pool.so libthread_pool.a

all: $(TARGETS)

tests/test: tests/test.o src/thread_pool.o
src/threadpool.o: src/thread_pool.c src/thread_pool.h
tests/test.o: tests/test.c src/thread_pool.h

shared: libthread_pool.so
static: libthread_pool.a

libthread_pool.so: src/thread_pool.c src/thread_pool.h
	$(CC) -shared -fPIC ${CFLAGS} -o $@ $< ${LDLIBS}

src/libthread_pool.o: src/thread_pool.c src/thread_pool.h
	$(CC) -c -fPIC ${CFLAGS} -o $@ $<

libthread_pool.a: src/libthread_pool.o
	ar rcs $@ $^

clean:
	rm -f $(TARGETS) *~ */*~ */*.o

test: $(TARGETS)
	./tests/test
