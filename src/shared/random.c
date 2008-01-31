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
 * A C-program for MT19937, with initialization improved 2002/1/26.
 * Coded by Takuji Nishimura and Makoto Matsumoto.
 *
 * Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. The names of its contributors may not be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Shared randomness support routines.
 *
 * The pseudo random number generator implemented here is the Mersenne Twister,
 * adapted for simplicity from the implementation from:
 * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
 *
 * We are providing our own PRNG here (rather than using the system's rand())
 * so that failure results may be reproduced independantly of an application
 * re-seeding if it wishes to do so. Conversely, our PRNG may be seeded without
 * affecting the application's.
 *
 * TODO the casts for constants to uint32_t are ugly. These are because 0x1UL
 * style constants assume uint32_t is unsigned long, which is may not be.
 *
 * XXX This is not thread-safe (forthcoming).
 *
 * $Id$
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <limits.h>

#include "random.h"

/* This value corresponds to the MT implementation */
#define GREAT_RAND_MAX 0xffffffffUL

/*
 * MT Period parameters
 * N corresponds to random.h's great_random_state.mt[]
 */
#define N 624
#define M 397
#define MATRIX_A (uint32_t) 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK (uint32_t) 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK (uint32_t) 0x7fffffffUL /* least significant r bits */

struct great_random_state great_random_failure;	/* global failure state */

/*
 * Draw the seed from the envrionment variable GREAT_RANDOM_SEED if present.
 * Otherwise, an arbitary default (5489) is used. We do not need to conform
 * to the srand() semantics of defaulting to 1, since this is not srand().
 */
static uint32_t
find_seed(void)
{
	char *s;
	char *ep;
	long l;

	/* Arbitary default */
	const uint32_t defaultseed = 5489;

	s = getenv("GREAT_RANDOM_SEED");
	if(!s) {
		return defaultseed;
	}

	l = strtol(s, &ep, 10);

	if(s[0] == '\0' || *ep != '\0') {
		fprintf(stderr, "Invalid integer for GREAT_RANDOM_SEED: %s\n", s);
		return defaultseed;
	}

	if((errno == ERANGE && (l == LONG_MAX || l == LONG_MIN))
		|| (l > UINT32_MAX || l < 0)) {
		fprintf(stderr, "Out of range for GREAT_RANDOM_SEED: %s\n", s);
		return defaultseed;
	}

	return l;
}

void
great_random_init(struct great_random_state *state)
{
	if(!state) {
		great_random_seed(&great_random_failure, find_seed());
	}
}

void
great_random_seed(struct great_random_state *state, uint32_t seed)
{
	state->mt[0] = seed & (uint32_t) 0xffffffffUL;

	for(state->mti = 1; state->mti < N; state->mti++) {
		state->mt[state->mti] = ((uint32_t) 1812433253UL
			* (state->mt[state->mti - 1] ^ (state->mt[state->mti - 1] >> 30))
			+ state->mti);

		/*
		 * See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
		 * In the previous versions, MSBs of the seed affect
		 * only MSBs of the array mt[].
		 */
		state->mt[state->mti] &= (uint32_t) 0xffffffffUL;
	}
}

/*
 * Generates a random number on the [0,0xffffffff]-interval.
 */
static uint32_t
genrand(struct great_random_state *state)
{
	uint32_t y;

	if(!state) {
		state = &great_random_failure;
	}

	/* mag01[x] = x * MATRIX_A  for x = 0,1 */
	static uint32_t mag01[2] = { 0, MATRIX_A };	/* TODO const? */

	/* generate N words at one time */
	if(state->mti >= N) {
		int kk;

		for(kk = 0; kk < N - M; kk++) {
			y = (state->mt[kk] & UPPER_MASK) | (state->mt[kk + 1] & LOWER_MASK);
			state->mt[kk] = state->mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}

		for( ; kk < N - 1; kk++) {
			y = (state->mt[kk] & UPPER_MASK) | (state->mt[kk + 1] & LOWER_MASK);
			state->mt[kk] = state->mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}

		y = (state->mt[N - 1] & UPPER_MASK) | (state->mt[0] & LOWER_MASK);
		state->mt[N - 1] = state->mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		state->mti = 0;
	}

	y = state->mt[state->mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & (uint32_t) 0x9d2c5680UL;
	y ^= (y << 15) & (uint32_t) 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}

/*
 * Randomly return true with a probability of p.
 */
static bool
probability(struct great_random_state *state, double p)
{
	return p < (double) genrand(state) / GREAT_RAND_MAX;
}

bool
great_random_bool(struct great_random_state *state)
{
	const char *s;
	double d;
	char *ep;

	s = getenv("GREAT_PROBABILITY");
	if(!s) {
		/* default */
		return probability(state, 0.5);
	}

	d = strtod(s, &ep);

	if(ep - s != (ptrdiff_t)strlen(s)) {
		fprintf(stderr, "Invalid double for GREAT_PROBABILITY: %s\n", s);
	}

	if(d >= HUGE_VAL || d <= -HUGE_VAL) {
		fprintf(stderr, "Overflow for GREAT_PROBABILITY: %s\n", s);
	}

	/* XXX test for underflow? */

	return probability(state, d);
}

unsigned int
great_random_choice(unsigned int range)
{
	/* See comp.lang.c FAQ 13.16 */
	unsigned int x = (GREAT_RAND_MAX) / range;
	unsigned int y = x * range;
	uint32_t r;

	do {
		r = genrand(&great_random_failure);
	} while(r >= y);

	return r / x;
}

