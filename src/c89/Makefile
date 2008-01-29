# $Id$

TARGETS = wrap.o stdlib.o

libgreat_c89.so: $(TARGETS)
	$(CC) -nostdlib -shared -fPIC -o $@ $(TARGETS) -L../shared -lshared

clean:
	rm -f $(TARGETS) libgreat_c89.so

