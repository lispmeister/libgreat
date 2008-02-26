# Library test programs
#
# These little programs illustrate the behaviour of specific library calls.
# They serve as tests for the various wrappers, demonstrating the repeatability
# of failure conditions. As such the results are most interesting when compared
# both with and without the wrapper libraries present.
#
# Execute along the lines of:
# GREAT_PROBABILITY=0.7 LD_PRELOAD=../src/c99/libgreat_c99.so \
# 	./whatever_test <arguments>
#
# As these are small specific tests, output will probably only make sense by
# reading the source.
#
# $Id$

REMOVE = rm -f

CFLAGS = -fPIC -std=c89 -Wall -Werror

TARGETS = malloc_test rand_test fopen_test

all: $(TARGETS)

clean:
	$(REMOVE) $(TARGETS)

fopen_test: fopen_test.o
	$(CC) $(CFLAGS) -o $@ fopen_test.o

malloc_test: malloc_test.o
	$(CC) $(CFLAGS) -o $@ malloc_test.o

rand_test: rand_test.o
	$(CC) $(CFLAGS) -o $@ rand_test.o

