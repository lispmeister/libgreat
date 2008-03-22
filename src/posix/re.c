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
 * Regular expressions.
 *
 * $Id$
 */

#include <stdbool.h>
#include <stdlib.h>
#include <regex.h>
#include <assert.h>

#include "../re.h"

struct great_re {
	regex_t preg;
};

void
great_re_error(int err, char *buf, size_t len)
{
	assert(buf);
	assert(len > 0);
	assert(err != 0);

	/*
	 * preg is passed as NULL as regerror() would expect this to be the most
	 * recently called regexp by regexec(); that cannot be guarenteed here,
	 * it is omitted at the cost of less helpful error messages.
	 */
	regerror(err, NULL, buf, len);
}

struct great_re *
great_re_comp(const char *rs, int *err)
{
	int e;
	struct great_re *new;

	assert(rs);

	new = malloc(sizeof *new);
	if (!new) {
		return NULL;
	}

	e = regcomp(&new->preg, rs, REG_EXTENDED | REG_NOSUB);

	if (err) {
		*err = e;
	}

	return new;
}

void
great_re_free(struct great_re *re)
{
	assert(re);

	regfree(&re->preg);
}

bool
great_re_match(struct great_re *re, const char *s)
{
	int e;

	e = regexec(&re->preg, s, 0, NULL, 0);
	if(0 == e) {
		return true;
	}

	if(REG_NOMATCH == e) {
		return false;
	}

	/* TODO handle error */
	return false;
}

