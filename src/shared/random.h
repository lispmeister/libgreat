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

#ifndef GREAT_SHARED_RANDOM_H
#define GREAT_SHARED_RANDOM_H

#include <stdbool.h>
#include <stdint.h>

/*
 * The state maintained for the PRNG. This provides a mechanism for multiple
 * simultaneously and isolated generators which do not interfere with each
 * other's sequences.
 *
 * Consider this struct's members private. It is not a forward declaration
 * for convenience of automatic allocation only.
 */
struct great_random_state {
	/*
	 * XXX these should be thread-local, or protected by mutex. This would be
	 * per API, since the knowledge of threads is external to random.c.
	 *
	 * TODO what is the behaviour per-thread?
	 */
    uint32_t mt[624]; /* The state vector. See random.c:N */
    uint32_t mti;
};

extern struct great_random_state great_random_failure;	/* global failure state */

/*
 * Initialise a PRNG state. This must be called before use. If the given state
 * is NULL, the global failure state (used for great_random_bool() and
 * great_random_choice() (which is maintained privately for their convenience)
 * is initialised.
 *
 * If the global failure state is initialised (that is, state is NULL), it is
 * also seeded. This is not true for other states. The seed for the global
 * state is taken from the environment (given as GREAT_RANDOM_SEED if present),
 * or an arbitary default (5489).
 */
void
great_random_init(struct great_random_state *state);

/*
 * Initialise the pseudo random number generator with a seed.
 * If not explicitly called, an arbitary constant seed will be
 * used (5489).
 */
void
great_random_seed(struct great_random_state *state, uint32_t seed);

/*
 * Randomly return true with a probability given by the environment
 * variable GREAT_PROBABILITY. If this is not set, the probability
 * defaults to 0.5.
 */
bool
great_random_bool(struct great_random_state *state);

/*
 * Randomly return a value in the range 0 to max - 1 inclusive.
 * The PRNG state used is great_random_failure.
 */
unsigned int
great_random_choice(unsigned int range);

/*
 * Generates a random long in the range LONG_MIN to LONG_MAX inclusive.
 */
long int
great_random_long(struct great_random_state *state);

/*
 * Generates a random int in the range INT_MIN to INT_MAX inclusive.
 */
int
great_random_int(void);

#endif

