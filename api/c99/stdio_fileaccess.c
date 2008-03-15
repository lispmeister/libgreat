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
 * C99 <stdio.h>
 * 7.19.5 File access functions
 *
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wrap.h"
#include "../../src/shared/subset.h"
#include "../../src/shared/log.h"

/*
 * CLC FAQ 13.8
 * Compare strings via pointers
 */
static int
pstrcmp(const void *p1, const void *p2)
{
	return strcmp(* (char * const *) p1, * (char * const *) p2);
}

/* C99 7.19.5.3 The fopen function */
FILE *
fopen(const char * restrict filename, const char * restrict mode)
{
	if (!great_subset("stdio:fileaccess:fopen")) {
		return great_c99.fopen(filename, mode);
	}

	/* P3 The argument mode points to a string. If the string is one of the
	 *    following ... otherwise the behaviour is undefined. */
	char *modes[] = {
		"r", "w", "a", "rb", "wb", "ab", "r+", "w+", "a+",
		"r+b", "rb+", "w+b", "wb+", "a+b", "ab+"
	};

	/* Sort for bsearch; we do not assume the execution character set */
	qsort(modes, sizeof modes / sizeof *modes, sizeof *modes, pstrcmp);

	/* I'm passing &mode here just so I can re-use pstrcmp() */
	if(!bsearch((void *) &mode, modes, sizeof modes / sizeof *modes,
		sizeof *modes, pstrcmp)) {
		great_ub("stdio:fileaccess:fopen", "7.19.5.3 P3",
			"Unrecognised mode: \"%s\"", mode);

		/* UB */
		abort();
	}

	great_log(GREAT_LOG_DEFAULT, "stdio:fileaccess:fopen", NULL);

	return great_c99.fopen(filename, mode);
}

