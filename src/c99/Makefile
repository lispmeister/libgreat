# $Id$

CFLAGS = -fPIC -std=c99 -Wall -Werror

TARGETS = wrap.o stdlib_prng.o stdlib_memory.o stdio_fileaccess.o

libgreat_c99.so: $(TARGETS)
	ld -o $@ -shared -o $@ $(TARGETS) -L../shared -lshared

clean:
	rm -f $(TARGETS) libgreat_c99.so

# TODO centralise tests?
rand_test: rand_test.o
	$(CC) $(CFLAGS) -o $@ rand_test.o

fopen_test: fopen_test.o
	$(CC) $(CFLAGS) -o $@ fopen_test.o

test: libgreat_c99.so rand_test fopen_test
	GREAT_PROBABILITY=1 LD_LIBRARY_PATH=. LD_PRELOAD=libgreat_c99.so ./rand_test

