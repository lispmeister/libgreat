# Compile a static library.
#
# $Id$

CFLAGS += -I $(SRC) $(SFLAGS)
LDFLAGS += -L $(SRC)/shared -L $(SRC)

CLEAN += $(LIB).a

all: $(LIB).a

$(LIB).a: $(TARGETS)
	ar r $@ $(TARGETS)
	ranlib $@

