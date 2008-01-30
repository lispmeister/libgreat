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
 * so that our APIs may wrap rand() (thus adjusting its behaviour to the
 * permissiable extremes) without affecting our internal descisions for
 * failure probability.
 *
 * Additionally, maintaining an isolated seed from libc's rand() means that
 * failure results may be reproduced independantly of an application re-seeding
 * if it wishes to do so. Conversely, our PRNG may be seeded without affecting
 * the application's.
 *
 * TODO Much of this code seems to assume that uint32_t >= 32-bit.
 * perhaps use uint32_t internally, instead.
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
#include <math.h>

/* This value corresponds to the MT implementation */
#define GREAT_RAND_MAX 0xffffffffUL

/*
 * MT Period parameters
 */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

/*
 * XXX these should be thread-local, or protected by mutex. This would be
 * per API, since the knowledge of threads is external to random.c.
 * Therefore these can be passed into great_random_seed() as a struct random.h
 * exports.
 */
static uint32_t mt[N];	/* the array for the state vector  */
static int mti = N + 1;	/* mti == N + 1 means mt[N] is not initialized */

void
great_random_seed(uint32_t seed)
{
	mt[0] = seed & 0xffffffffUL;

	for(mti = 1; mti < N; mti++) {
		mt[mti] = (1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);

		/*
		 * See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
		 * In the previous versions, MSBs of the seed affect
		 * only MSBs of the array mt[].
		 */
		mt[mti] &= 0xffffffffUL;
	}
}

/*
 * Generates a random number on the [0,0xffffffff]-interval.
 */
static uint32_t
genrand(void)
{
	uint32_t y;

	/* mag01[x] = x * MATRIX_A  for x = 0,1 */
	static uint32_t mag01[2] = { 0x0UL, MATRIX_A };	/* TODO const? */

	/* generate N words at one time */
	if(mti >= N) {
		int kk;

		/*
		 * If great_random_seed() has not been called, a default initial
		 * seed is used.
		 */
		if(mti == N + 1) {
			/* TODO draw seed from the envrionment if present */
			great_random_seed(5489UL);
		}

		for(kk = 0; kk < N - M; kk++) {
			y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
			mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}

		for( ; kk < N - 1; kk++) {
			y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
			mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}

		y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
		mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		mti = 0;
	}

	y = mt[mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}

/*
 * Randomly return true with a probability of p.
 */
static bool
probability(double p)
{
	return p < (double) genrand() / GREAT_RAND_MAX;
}

bool
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
	unsigned int x = (GREAT_RAND_MAX) / range;
	unsigned int y = x * range;
	uint32_t r;

	do {
		r = genrand();
	} while(r >= y);

	return r / x;
}

