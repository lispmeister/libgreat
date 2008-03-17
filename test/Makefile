# Library test programs
#
# These little programs illustrate the behaviour of specific library calls.
# They serve as tests for the various wrappers, demonstrating the repeatability
# of failure conditions. As such the results are most interesting when compared
# both with and without the wrapper libraries present.
#
# Execute along the lines of:
# GREAT_PROBABILITY=0.7 LD_PRELOAD=../api/c99/libgreat_c99.so \
# 	./whatever_test <arguments>
#
# As these are small specific tests, output will probably only make sense by
# reading the source.
#
# $Id$

MK = ../mk

TESTS = malloc_test rand_test fopen_test ctype_test
CLEAN += $(TESTS)

all: $(TESTS)

include $(MK)/cc.mk
include $(MK)/rules.mk

