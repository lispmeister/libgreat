/*
 * Copyright 2008 Katherine Flavel. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither the name of the author nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * C99 <stdlib.h>
 * 7.20.3 Memory management functions
 *
 * $Id$
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "wrap.h"
#include "../../src/shared/random.h"
#include "../../src/shared/subset.h"
#include "../../src/shared/log.h"

/*
 * Here we're after a valid pointer (that is, one which malloc may return) which
 * points to an address that may not be read from or written to.
 *
 * C99 6.5.5 P8 If both the pointer operand and the result point ... one past
 * the last element of the array object, the evaluation shall not produce an
 * overflow.
 */
char great_nothing[] = "";

/* C99 7.20.3.2 The free function */
void
free(void *ptr) {
	if (!great_subset("stdlib:memory:malloc")
	&& !great_subset("stdlib:memory:realloc")
	&& !great_subset("stdlib:memory:free")) {
		great_c99.free(ptr);
		return;
    }

	if(ptr == great_nothing + 1) {
		great_log(GREAT_LOG_INFO, "stdlib:memory:free",
			"Handling great_nothing");
		return;
	}

	great_log(GREAT_LOG_DEFAULT, "stdio:memory:free", NULL);
	great_c99.free(ptr);
}

/* C99 7.20.3.3 The malloc function */
void *
malloc(size_t size)
{
	if (!great_subset("stdlib:memory:malloc")) {
		return great_c99.malloc(size);
	}

	if (!great_random_probability(NULL)) {
		great_log(GREAT_LOG_DEFAULT, "stdlib:memory:malloc", NULL);
		return great_c99.malloc(size);
	}

	switch(great_random_choice(1u + (size == 0))) {
	case 0:
		/* P3 The malloc function returns either a null pointer... */
		great_ib("stdio:memory:malloc", "7.20.3.3 P3", "Returning NULL");
		return NULL;

	case 1:
		/* J.2 IDB: The amount of storage allocated by a successful call to
		 * malloc when 0 bytes was requested */
		/* XXX IDB: we could also return an arbitary amount of memory here */
		great_ib("stdio:memory:malloc", "7.20.3.3 P3",
			"Returning great_nothing");
		assert(size == 0);
		return great_nothing + 1;

	default:
		assert(!"unrecognised great random choice");
	}

	/* NOTREACHED */
	return NULL;
}

/* C99 7.20.3.4 The realloc function */
void *
realloc(void *ptr, size_t size)
{
	if (!great_subset("stdlib:memory:realloc")) {
		return great_c99.realloc(ptr, size);
    }

	if(!great_random_probability(NULL)) {
		great_log(GREAT_LOG_DEFAULT, "stdlib:memory:realloc", NULL);
		return great_c99.realloc(ptr, size);
	}

	/* P3 If ptr is a null pointer, the realloc function behaves like like
	 *    malloc function for the specified size. */
	if(ptr == NULL) {
		great_ib("stdio:memory:realloc", "7.20.3.4 P3",
			"Returning malloc()");
		return malloc(size);
	}

	/* P4 The realloc function returns a pointer to the new object */
	switch(great_random_choice(2)) {
	case 0:
		/* P4 The realloc function returns ... a null pointer */
		great_ib("stdio:memory:realloc", "7.20.3.4 P4", "Returning NULL");
		return NULL;

	case 1:
		/* a different address */
		{
			void *p;

			/* XXX call our callback, here */
			p = malloc(size);
			if(!p) {
				great_perror("stdlib:memory:realloc", "malloc");

				great_ib("stdio:memory:realloc", "7.20.3.4 P4",
					"Returning NULL");

				return NULL;
			}

			/* TODO retry (before freeing) if it's not is different */

			/* P2 The contents of the new object shall be the same as that of
			 *    the old object, up to the lesser of the new and old sizes. */
			memcpy(p, ptr, size);
			free(ptr);

			great_ib("stdio:memory:realloc", "7.20.3.4 P2",
				"Returning different address");

			return p;
		}

	default:
		assert(!"unrecognised great random choice");
	}

	/* NOTREACHED */
	return NULL;
}

