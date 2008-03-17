# Compile a dynamic library.
#
# $Id$

CFLAGS += -I $(SRC) $(SFLAGS)
LDFLAGS += -L $(SRC)/shared -L $(SRC)/port

CLEAN += $(LIB).so

all: $(LIB).so $(LIB).a

$(LIB).so: $(TARGETS)
	ld -o $@ -shared $(TARGETS) \
		-L $(SRC)/shared -lshared \
		-L $(SRC)/port -lport

