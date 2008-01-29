/*
 * C89 <stdlib.h>
 *
 * $Id$
 *
 * Note that we do not attempt to provide semantic checking such as asserting
 * that pointer values are valid. Use this in combination with a mechanism
 * which does, to bring in such checks - perhaps libefence or similar.
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include "wrap.h"
#include "../shared/random.h"

void *
malloc(size_t size)
{
	if(!great_random_success()) {
		errno = ENOMEM;
		return NULL;
	}

	return great_c89.malloc(size);
}

/* XXX No mention of errno in the standard? */
void *
realloc(void *ptr, size_t size)
{
	void *p;

	if(ptr == NULL) {
		return malloc(size);
	}

	if(!great_random_success()) {
		errno = ENOMEM;
		return NULL;
	}

	switch(great_random_choice(2)) {
	case 0:
		/* possibly the same address */
		return great_c89.realloc(ptr, size);

	case 1:
		/* a different address */
		{
			void *p;

			p = malloc(size);
			if(!p) {
				errno = ENOMEM;
				return NULL;
			}

			memcpy(p, ptr, size);
			free(ptr);

			return p;
		}

	default:
		assert(!"unrecognised great random choice");
	}

	/* NOTREACHED */
}

