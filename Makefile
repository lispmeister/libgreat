# $Id$

all:
	cd src && $(MAKE)
	cd api && $(MAKE)
	cd test && $(MAKE)

clean:
	cd src && $(MAKE) clean
	cd api && $(MAKE) clean
	cd test && $(MAKE) clean

