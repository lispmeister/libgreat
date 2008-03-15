# $Id$

MK = ../../mk
SRC = ../../src

LIB = libshared

TARGETS = wrap.o random.o subset.o log.o misc.o
TESTS = random_test log_test
CLEAN += $(TESTS)

all: $(LIB).a $(TESTS)

test: $(TESTS)
	GREAT_RANDOM_SEED=12345 ./random_test 5
	GREAT_LOG=- ./log_test

random_test: random_test.o random.o log.o subset.o misc.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) \
		random_test.o random.o log.o subset.o misc.o -lport

log_test: log_test.o log.o subset.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) \
		log_test.o log.o subset.o misc.o -lport

include $(MK)/rules.mk
include $(MK)/ar.mk
include $(MK)/exec.mk

