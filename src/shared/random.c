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
	return (double)rand() <= (double)RAND_MAX * p;
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

	if(d == HUGE_VAL || d == -HUGE_VAL) {
		fprintf(stderr, "Overflow for GREAT_PROBABILITY: %s\n", s);
	}

	/* XXX test for underflow? */

	return probability(d);
}

