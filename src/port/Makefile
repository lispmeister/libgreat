# $Id$

PORT ?= posix

all:
	cd $(PORT) && $(MAKE)
	ln -sf $(PORT)/libport.a .

clean:
	cd $(PORT) && $(MAKE) clean
	rm -f libport.a

