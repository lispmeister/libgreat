# $Id$

CFLAGS = -fPIC -std=c89 -Wall -Werror

TARGETS = wrap.o stdlib.o

libgreat_c89.so: $(TARGETS)
	ld -o $@ -shared -o $@ $(TARGETS) -L../shared -lshared

clean:
	rm -f $(TARGETS) libgreat_c89.so

stdlib_test: stdlib_test.o
	$(CC) $(CFLAGS) -o $@ stdlib_test.o

test: stdlib_test
	GREAT_PROBABILITY=1 LD_LIBRARY_PATH=. LD_PRELOAD=libgreat_c89.so ./stdlib_test

