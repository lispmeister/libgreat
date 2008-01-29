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

#include "wrap.h"
#include "../shared/random.h"

/* TODO paragraph numbers for P? */

/* C89 4.10.3.3 The malloc function */
void *
malloc(size_t size)
{
	/* P? ...either a null pointer */
	if(!great_random_success()) {
		return NULL;
	}

	/* P? ...or a pointer to the allocated space */
	return great_c89.malloc(size);
}

/* C89 4.10.3.4 The realloc function */
void *
realloc(void *ptr, size_t size)
{
	void *p;

	/* P? If ptr is a null pointer, the realloc function
	 *    behaves like the malloc function for the specified size. */
	if(ptr == NULL) {
		return malloc(size);
	}

	/* P? If size is zero and ptr is not a null pointer, the object
	 *    it points to is freed. */
	if(size == 0) {
		free(ptr);

		/* C89 does not enforce that NULL is returned (I think...) */
		switch(great_random_choice(2)) {
		case 0:
			return NULL;

		case 1:
			return ptr;

		default:
			assert(!"unrecognised great random choice");
		}
	}

	if(!great_random_success()) {
		return NULL;
	}

	/* P? The realloc function returns either a null pointer or a pointer to
	 *    the possibly moved allocated space. */
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
				return NULL;
			}

			/* P? The contents of the object shall be unchanged up to the
			 *    lesser of the new and old sizes. */
			memcpy(p, ptr, size);
			free(ptr);

			return p;
		}

	default:
		assert(!"unrecognised great random choice");
	}

	/* NOTREACHED */
}

