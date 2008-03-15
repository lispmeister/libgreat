# $Id$

CFLAGS = -fPIC -std=c99 -Wall -Werror

TARGETS = wrap.o sys_time.o

all: libgreat_bsd42.so libgreat_bsd42.a

libgreat_bsd42.so: $(TARGETS)
	ld -o $@ -shared -o $@ $(TARGETS) -L../shared -lshared

libgreat_bsd42.a: $(TARGETS)
	ar r $@ $(TARGETS)
	ranlib $@

clean:
	rm -f $(TARGETS) libgreat_bsd42.so

