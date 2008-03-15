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
 * BSD 4.4 <string.h>
 *
 * $Id$
 */

/* Required for strdup() on GNU systems */
#define _GNU_SOURCE

#include <errno.h>
#include <stddef.h>

#include "wrap.h"
#include "../shared/subset.h"
#include "../shared/random.h"
#include "../shared/log.h"

char *
strdup(const char *str)
{
	if (!great_subset("string:memory:strdup")) {
		return great_bsd44.strdup(str);
	}

	if (!great_random_bool(NULL)) {
		great_log(GREAT_LOG_DEFAULT, "string:memory:strdup", NULL);
		return great_bsd44.strdup(str);
	}

	great_ib("string:memory:strdup", "strdup(3)", "Returning NULL");

	/*
	 * strdup(3): If insufficient memory is available, NULL is returned.
	 *
	 * strdup(3): The strdup() function may fail and set the external variable
	 * errno for any of the errors specified for the library function malloc(3).
	 *
	 * malloc(3): If malloc() fails, a NULL pointer is returned, and the errno
	 * variable is set to ENOMEM.
	 */
	errno = ENOMEM;
	return NULL;
}

