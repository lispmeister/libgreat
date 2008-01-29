/*
 * $Id$
 */

#include "wrap.h"
#include "../shared/wrap.h"

struct great_c89 great_c89;

void
_init(void) {
	/* <stdlib.h> */
	great_c89.malloc	= great_wrap_resolve("malloc");
	great_c89.realloc	= great_wrap_resolve("realloc");
}

