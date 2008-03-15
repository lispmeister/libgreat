# $Id$

CFLAGS = -fPIC -std=c99 -Wall -Werror

TARGETS = wrap.o stdlib_prng.o stdlib_memory.o stdio_fileaccess.o ctype.o

all: libgreat_c99.so libgreat_c99.a

libgreat_c99.so: $(TARGETS)
	ld -o $@ -shared -o $@ $(TARGETS) -L../../src/shared -lshared -L../../src/port -lport

libgreat_c99.a: $(TARGETS)
	ar r $@ $(TARGETS) ../../src/shared/libshared.a ../../src/port/libport.a
	ranlib $@

clean:
	rm -f $(TARGETS) libgreat_c99.so

