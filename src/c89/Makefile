# $Id$

TARGETS = wrap.o stdlib.o

libgreat_c89.so: $(TARGETS)
	$(CC) -nostdlib -shared -fPIC -o $@ $(TARGETS) -L../shared -lshared

clean:
	rm -f $(TARGETS) libgreat_c89.so

stdlib_test: stdlib_test.o
	$(CC) -o $@ stdlib_test.o

test: stdlib_test
	GREAT_PROBABILITY=1 LD_LIBRARY_PATH=. LD_PRELOAD=libgreat_c89.so ./stdlib_test

