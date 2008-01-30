# $Id$

CFLAGS = -fPIC -std=c89 -Wall -Werror

TARGETS = wrap.o random.o

libshared.a: $(TARGETS)
	ar r $@ $(TARGETS)

clean:
	rm -f $(TARGETS)

test: random_test
	./random_test 5

random_test: random_test.o random.o
	$(CC) $(CFLAGS) -o $@ random_test.o random.o

