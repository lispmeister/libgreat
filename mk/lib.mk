# Compile a dynamic library.
#
# $Id$

CFLAGS += -I $(SRC) $(SFLAGS)
LDFLAGS += -L $(SRC)/shared -L $(SRC)

CLEAN += $(LIB).so

all: $(LIB).so $(LIB).a

$(LIB).so: $(TARGETS)
	ld -o $@ -shared $(TARGETS) \
		$(LDFLAGS) -lshared -lport

