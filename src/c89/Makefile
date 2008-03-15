# $Id$

CFLAGS = -fPIC -std=c89 -Wall -Werror

TARGETS = wrap.o stdlib_memory.o

all: libgreat_c89.so libgreat_c89.a

libgreat_c89.so: $(TARGETS)
	ld -o $@ -shared -o $@ $(TARGETS) -L../shared -lshared -L../port -lport

libgreat_c89.a: $(TARGETS)
	ar r $@ $(TARGETS) ../shared/libshared.a ../port/libport.a
	ranlib $@

clean:
	rm -f $(TARGETS) libgreat_c89.so

