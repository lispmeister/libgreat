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
 * C99 <stdlib.h>
 *
 * $Id$
 */

#include <stdlib.h>

#include "wrap.h"
#include "../shared/random.h"

/* C99 7.20.2.1 The rand function */
int
rand(void)
{
	/* P4 The rand function returns a pseudo-random integer */
	/*
	 * This wrapper has the extra caveat that our descision weather to fail or
	 * not must also be repeatable by srand() (else the sequence would differ
	 * given the same seed). Hence we make that descision part of the same
	 * sequence we return, by simply using rand().
	 *
	 */
	if(!great_random_bool(&great_c99.successstate)) {
		/*
		 * A constant is returned based on the premise that a sequence of
		 * random numbers may repeat one number infinitely. Seven is one of
		 * my favorite numbers.
		 */
		return 7;
	}

	return great_c99.rand();
}

/* C99 7.20.2.2 The srand function */
void
srand(unsigned int seed)
{
	/* 7.20.2.2 P2 If srand is then called with the same seed value, the
	 * sequence of pseudo-random numbers shall be repeated. */
	great_c99.srand(seed);

	/*
	 * For our wrapper, this additionally means that our failure descisions also
	 * must be repeated, hence we also re-seed that PRNG.
	 */
	great_random_seed(&great_c99.successstate, seed);
}

