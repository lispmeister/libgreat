/*
 * $Id$
 */

#include "wrap.h"
#include "../shared/wrap.h"

struct great_c89 great_c89;

void
_init(void) {
	/* <stdlib.h> */
	great_c89.malloc = wrap_resolve("malloc");
}

