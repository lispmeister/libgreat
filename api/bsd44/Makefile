# $Id$

CFLAGS = -fPIC -std=c99 -Wall -Werror

TARGETS = wrap.o string.o

all: libgreat_bsd44.so libgreat_bsd44.a

libgreat_bsd44.so: $(TARGETS)
	ld -o $@ -shared -o $@ $(TARGETS) -L../../src/shared -lshared -L../../src/port -lport

libgreat_bsd44.a: $(TARGETS)
	ar r $@ $(TARGETS) ../../src/shared/libshared.a ../../src/port/libport.a
	ranlib $@

clean:
	rm -f $(TARGETS) libgreat_bsd44.so

