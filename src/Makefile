# $Id$

PORT ?= posix

all:
	cd port/$(PORT) && $(MAKE)
	cd shared && $(MAKE)

clean:
	cd port/$(PORT) && $(MAKE) clean
	cd shared && $(MAKE) clean

