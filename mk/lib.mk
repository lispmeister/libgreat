# Compile a dynamic library.
# TODO have this include ar.mk
#
# $Id$

CFLAGS = $(SFLAGS) -I $(SRC)
LDFLAGS = -L $(SRC)/shared -L $(SRC)/port

CLEAN += $(LIB).so

all: $(LIB).so $(LIB).a

$(LIB).so: $(TARGETS)
	ld -o $@ -shared $(TARGETS) \
		-L $(SRC)/shared -lshared \
		-L $(SRC)/port -lport

include $(MK)/cc/$(CC).mk

