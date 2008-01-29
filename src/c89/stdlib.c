/*
 * C89 <stdlib.h>
 *
 * $Id$
 */

#include <stdlib.h>

#include "wrap.h"

void *
malloc(size_t size)
{
	return great_c89.malloc(size);
}

