/*
 * C89 <stdlib.h>
 *
 * $Id$
 */

#include <stdlib.h>

#include "wrap.h"
#include "../shared/random.h"

void *
malloc(size_t size)
{
	if(!great_random_success()) {
		return NULL;
	}

	return great_c89.malloc(size);
}

