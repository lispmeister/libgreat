# $Id$

CFLAGS = -fPIC -std=c89 -Wall -Werror

TARGETS = wrap.o random.o subset.o log.o

libshared.a: $(TARGETS)
	ar r $@ $(TARGETS)

clean:
	rm -f $(TARGETS) *_test

test: random_test
	GREAT_RANDOM_SEED=12345 ./random_test 5

random_test: random_test.o random.o
	$(CC) $(CFLAGS) -o $@ random_test.o random.o

log_test: log_test.o log.o
	$(CC) $(CFLAGS) -o $@ log_test.o log.o

