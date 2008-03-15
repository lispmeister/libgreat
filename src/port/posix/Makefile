# $Id$

CFLAGS = -fPIC -std=c99 -Wall -Werror

TARGETS = time.o

all: ../libport.a

../libport.a: $(TARGETS)
	ar r $@ $(TARGETS)
	ranlib $@

clean:
	rm -f $(TARGETS) ../libport.a

