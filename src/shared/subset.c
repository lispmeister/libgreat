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
 * API subset selection.
 *
 * $Id$
 */

#include <stdbool.h>
#include <stdlib.h>
#include <regex.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <unistd.h> /* XXX debugging only */

#include "subset.h"

struct subset {
	regex_t preg;

	struct subset *next;
};

/*
 * This is the list of all subsets which are to be used. Each element in the
 * list contains a compiled regular expression for matching that subset.
 */
static struct subset *subsets;

static bool
cisdelim(int c)
{
	char *s = "[^$()\\:|-.";

	/*
	 * These characters are punctuation, but may not be used as delimiters,
	 * since they are special characters for regular expressions.
	 */
	if (strchr(s, c)) {
		return false;
	}

	/* any remaining punctuation will do */
	return ispunct(c);
}

static bool
single(const char *rs)
{
	int e;
	regex_t re;
	struct subset *new;

	assert(rs);

	if (!*rs) {
		return false;
	}

	e = regcomp(&re, rs, REG_EXTENDED | REG_NOSUB);
	if (e) {
		char buf[128];

		regerror(e, &re, buf, sizeof buf);
		/* TODO explain why we use write. make our own minimal printf in shared/, instead */
		write(STDERR_FILENO, buf, sizeof buf);

		regfree(&re);
		return false;
	}

	new = malloc(sizeof *new);
	if (!new) {
		regfree(&re);
		return false;
	}

	/* push to head of list; order is irrelevant */
	new->preg = re;
	new->next = subsets;
	subsets = new;

	return true;
}

bool
great_subset_init(void)
{
	char *restr;

	/* TODO consider freeing previous subsets, so we can be re-called for environment changes */
	/* TODO add a _fini mechanism to free on exit */

	restr = getenv("GREAT_SUBSETS");

	/* default to matching everything */
	if (!restr) {
		restr = ".";
	}

	/* a single regular expression */
	if (!cisdelim(*restr)) {
		return single(restr);
	}

	/* non-special punctation delimits a list of regexes */
	{
		char *lasts;
		char *re;
		char *rel;
		char d[] = { *restr, '\0' };

		rel = strdup(restr);
		if (!rel) {
			return false;
		}

		strtok_r(rel, d, &lasts);
		for (re = strtok_r(rel, d, &lasts); re; re = strtok_r(NULL, d, &lasts)) {
			if (!single(re)) {
				/* TODO set errno and such, for things like this */
				free(rel);
				return false;
			}
		}

		free(rel);
		return true;
	}
}

bool
great_subset(const char *name)
{
	struct subset *subset;

	assert(name);

	/* TODO sanity check name */

	for (subset = subsets; subset; subset = subset->next) {
		if (!regexec(&subset->preg, name, 0, NULL, 0)) {
			return true;
		}
	}

	return false;
}

