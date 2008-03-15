# Compile a static library.
#
# $Id$

CFLAGS = -I $(SRC)
LDFLAGS = -L $(SRC)/shared -L $(SRC)/port

CLEAN += $(LIB).a

all: $(LIB).a

$(LIB).a: $(TARGETS)
	ar r $@ $(TARGETS)
	ranlib $@

include $(MK)/cc/$(CC).mk

