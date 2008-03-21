# $Id$

PORT ?= posix

all:
	cd $(PORT) && $(MAKE)
	ln -sf $(PORT)/libport.a .
	cd shared && $(MAKE)

clean:
	cd $(PORT) && $(MAKE) clean
	rm -f libport.a
	cd shared && $(MAKE) clean

