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
 * C89 <stdlib.h>
 * 4.10.3 Memory management functions
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
#include "../../src/shared/random.h"
#include "../../src/shared/subset.h"
#include "../../src/shared/log.h"

/* TODO paragraph numbers for P? */

/*
 * Here we're after a valid pointer (that is, one which malloc may return) which
 * points to an address that may not be read from or written to.
 *
 * TODO reference C89
 */
char great_nothing[] = "";

/* XXX free() is missing */

/* C89 4.10.3.3 The malloc function */
void *
malloc(size_t size)
{
	if (!great_subset("stdlib:memory:malloc")) {
		return great_c89.malloc(size);
	}

	if (!great_random_probability(NULL)) {
		great_log(GREAT_LOG_DEFAULT, "stdlib:memory:malloc", NULL);
		return great_c89.malloc(size);
	}

	/* P? ...either a null pointer */
	great_ib("stdlib:memory:malloc", "4.10.3.3 P?", "Returning NULL");
	return NULL;

	/* NOTREACHED */
	return NULL;
}

/* C89 4.10.3.4 The realloc function */
void *
realloc(void *ptr, size_t size)
{
	if (!great_subset("stdlib:memory:realloc")) {
		return great_c89.realloc(ptr, size);
	}

	if (!great_random_probability(NULL)) {
		great_log(GREAT_LOG_DEFAULT, "stdlib:memory:realloc", NULL);
		return great_c89.realloc(ptr, size);
	}

	/* P? If ptr is a null pointer, the realloc function
	 *    behaves like the malloc function for the specified size. */
	if(ptr == NULL) {
		great_ib("stdlib:memory:realloc", "4.10.3.4 P?",
			"Returning malloc()");
		return malloc(size);
	}

	/* P? If size is zero and ptr is not a null pointer, the object
	 *    it points to is freed. */
	if(size == 0) {
		free(ptr);

		/* C89 does not enforce that NULL is returned (I think...) */
		switch(great_random_choice(2)) {
		case 0:
			great_ib("stdlib:memory:realloc", "4.10.3.4 P?",
				"Returning NULL");
			return NULL;

		case 1:
			great_ib("stdlib:memory:realloc", "4.10.3.4 P?",
				"Returning great_nothing");
			return great_nothing + 1;

		default:
			assert(!"unrecognised great random choice");
		}
	}

	/* P? The realloc function returns either a null pointer or a pointer to
	 *    the possibly moved allocated space. */
	switch(great_random_choice(2)) {
	case 0:
		great_ib("stdlib:memory:realloc", "4.10.3.4 P?",
			"Returning NULL");
		return NULL;

	case 1:
		/* a different address */
		{
			void *p;

			p = malloc(size);
			if(!p) {
				great_perror("stdlib:memory:realloc", "malloc");

				great_ib("stdlib:memory:realloc", "4.10.3.4 P?",
					"Returning NULL");

				return NULL;
			}

			/* P? The contents of the object shall be unchanged up to the
			 *    lesser of the new and old sizes. */
			memcpy(p, ptr, size);
			free(ptr);

			great_ib("stdlib:memory:realloc", "7.20.3.4 P2",
				"Returning different address");

			return p;
		}

	default:
		assert(!"unrecognised great random choice");
	}

	/* NOTREACHED */
	return NULL;
}

