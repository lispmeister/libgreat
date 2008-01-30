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
 * Shared randomness support routines.
 *
 * $Id$
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
 * Randomly return true with a probability of p.
 */
static int
probability(double p)
{
	return p < (double)rand() / RAND_MAX;
}

int
great_random_success(void) {
	const char *s;
	double d;
	char *ep;

	s = getenv("GREAT_PROBABILITY");
	if(!s) {
		/* default */
		return probability(0.5);
	}

	d = strtod(s, &ep);

	if(ep - s != (ptrdiff_t)strlen(s)) {
		fprintf(stderr, "Invalid double for GREAT_PROBABILITY: %s\n", s);
	}

	if(d >= HUGE_VAL || d <= -HUGE_VAL) {
		fprintf(stderr, "Overflow for GREAT_PROBABILITY: %s\n", s);
	}

	/* XXX test for underflow? */

	return probability(d);
}

unsigned int
great_random_choice(unsigned int range) {
	/* See comp.lang.c FAQ 13.16 */
	unsigned int x = (RAND_MAX + 1u) / range;
	unsigned int y = x * range;
	unsigned int r;

	do {
		r = (unsigned int) rand();
	} while(r >= y);

	return r / x;
}

