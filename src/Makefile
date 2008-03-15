# $Id$

PORT ?= posix

all:
	cd port/$(PORT) && $(MAKE)
	cd shared && $(MAKE)
	cd c89 && $(MAKE)
	cd c99 && $(MAKE)
	cd bsd42 && $(MAKE)
	cd bsd44 && $(MAKE)

clean:
	cd port/$(PORT) && $(MAKE) clean
	cd shared && $(MAKE) clean
	cd c89 && $(MAKE) clean
	cd c99 && $(MAKE) clean
	cd bsd42 && $(MAKE) clean
	cd bsd44 && $(MAKE) clean

