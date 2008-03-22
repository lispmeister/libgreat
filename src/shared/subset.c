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

/* Required for strtok_r */
#define _POSIX_C_SOURCE 199506L

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "subset.h"
#include "log.h"
#include "misc.h"
#include "../re.h"

struct subset {
	struct great_re *re;

	struct subset *next;
};

/*
 * This is the list of all subsets which are to be used. Each element in the
 * list contains a compiled regular expression for matching that subset.
 */
static struct subset *subsets;

/*
 * This is set non-zero if all subsets are disabled; see great_subset_disable()
 * and great_subset_enable(). This provides a scope-like mechanism, where code
 * disabling subsets may be nested inside code which may or may not have already
 * disabled subsets for its own purposes.
 */
unsigned int subsets_disabled;

static bool
cisdelim(int c)
{
	const char *s = "[^$()\\:|-.";

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
	struct great_re *re;
	struct subset *new;

	assert(rs);

	if (!*rs) {
		return false;
	}

	re = great_re_comp(rs, &e);
	if (!re) {
		char buf[128];

		great_re_error(e, buf, sizeof buf);
		great_log(GREAT_LOG_ERROR, "GREAT_SUBSETS",
			"%s; disregarding /%s/", buf, rs);
		great_re_free(re);
		return false;
	}

	errno = 0;
	new = malloc(sizeof *new);	/* XXX these should be our callbacks instead */
	if (!new) {
		great_perror("GREAT_SUBSETS", "malloc");
		great_re_free(re);
		return false;
	}

	/* push to head of list; order is irrelevant */
	new->re = re;
	new->next = subsets;
	subsets = new;

	great_log(GREAT_LOG_INFO, "GREAT_SUBSETS", "Registered subset: /%s/", rs);

	return true;
}

bool
great_subset_init(void)
{
	const char *restr;

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

		errno = 0;
		rel = great_strdup(restr);
		if (!rel) {
			great_perror("GREAT_SUBSETS", "great_strdup");
			return false;
		}

		for (re = strtok_r(rel, d, &lasts); re; re = strtok_r(NULL, d, &lasts)) {
			if (!single(re)) {
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

	if (subsets_disabled > 0) {
		return false;
	}

	/* TODO sanity check name */

	for (subset = subsets; subset; subset = subset->next) {
		if (!great_re_match(subset->re, name)) {
			return true;
		}
	}

	return false;
}

void
great_subset_disable(void)
{
	subsets_disabled++;

	assert(subsets_disabled > 0);
}

void
great_subset_enable(void)
{
	subsets_disabled--;
}

