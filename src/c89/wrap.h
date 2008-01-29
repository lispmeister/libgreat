/*
 * $Id$
 */

#ifndef GREAT_C89_WRAP_H
#define GREAT_C89_WRAP_H

#include <stdlib.h>

struct great_c89 {
	/* <stdlib.h> */
	void *(*malloc)(size_t size);
};

extern struct great_c89 great_c89;

extern void
_init(void);

#endif

