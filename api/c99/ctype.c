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
 * C99 <ctype.h>
 * 7.4.1 Character classification functions
 *
 * $Id$
 */

#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "wrap.h"
#include "../../src/shared/random.h"
#include "../../src/shared/subset.h"
#include "../../src/shared/log.h"

static void
checkrange(int c)
{
	/* 7.4 P1 In all cases the argument is an int, the value of which shall be
	 * representable as an unsigned char ... */
	if (c >= 0 && c <= UCHAR_MAX) {
		return;
	}

	/* ... or shall equal the value of the macro EOF. ... */
	if (EOF == c) {
		return;
	}

	/* ... If the argument has any other value, the behavior is undefined. */
	great_ub("stdlib:prng:rand", "7.4 P1",
		"character not representable as an unsigned char or EOF");
	/* UB */
	abort();
}

/*
 * The guts of the is*() functions, generalised.
 */
static int
xis(const char *subset, int (*fp)(int c), int c) {
	int x;

	assert(subset);
	assert(fp);

	if (!great_subset(subset)) {
		return fp(c);
	}

	if (!great_random_bool(NULL)) {
		great_log(GREAT_LOG_DEFAULT, subset, NULL);
		return fp(c);
	}

	checkrange(c);

	x = fp(c);
	if (0 == x) {
		/* 7.4.1 P1 The functions in this subclause return nonzero (true)
		 * if and only if the value of the argument c conforms to that in
		 * the description of the function. */
		great_ib(subset, "7.4.1 P1", "Returning 0");
		return 0;
	}

	/* 7.4.1 P1 The functions in this subclause return nonzero (true) */
	do {
		x = great_random_int();
	} while (0 == x);

	great_ib(subset, "7.4.1 P1", "Returning random non-zero value");
	return x;
}

/* C99 7.4.1.1 The isalnum function */
int
isalnum(int c)
{
	return xis("ctype:class:isalnum", great_c99.isalnum, c);
}

/* C99 7.4.1.2 The isalpha function */
int
isalpha(int c)
{
	return xis("ctype:class:isalpha", great_c99.isalpha, c);
}

/* C99 7.4.1.3 The isblank function */
int
isblank(int c)
{
	return xis("ctype:class:isblank", great_c99.isblank, c);
}

/* C99 7.4.1.4 The iscntrl function */
int
iscntrl(int c)
{
	return xis("ctype:class:iscntrl", great_c99.iscntrl, c);
}

/* C99 7.4.1.5 The isdigit function */
int
isdigit(int c)
{
	return xis("ctype:class:isdigit", great_c99.isdigit, c);
}

/* C99 7.4.1.6 The isgraph function */
int
isgraph(int c)
{
	return xis("ctype:class:isgraph", great_c99.isgraph, c);
}

/* C99 7.4.1.7 The islower function */
int
islower(int c)
{
	return xis("ctype:class:islower", great_c99.islower, c);
}

/* C99 7.4.1.8 The isprint function */
int
isprint(int c)
{
	return xis("ctype:class:isprint", great_c99.isprint, c);
}

/* C99 7.4.1.9 The ispunct function */
int
ispunct(int c)
{
	return xis("ctype:class:ispunct", great_c99.ispunct, c);
}

/* C99 7.4.1.10 The isspace function */
int
isspace(int c)
{
	return xis("ctype:class:isspace", great_c99.isspace, c);
}

/* C99 7.4.1.11 The isupper function */
int
isupper(int c)
{
	return xis("ctype:class:isupper", great_c99.isupper, c);
}

/* C99 7.4.1.12 The isxdigit function */
int
isxdigit(int c)
{
	return xis("ctype:class:isxdigit", great_c99.isxdigit, c);
}

