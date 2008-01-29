/*
 * Shared randomness support routines.
 *
 * $Id$
 */

#ifndef GREAT_SHARED_RANDOM_H
#define GREAT_SHARED_RANDOM_H

/*
 * Randomly return true with a probability given by the environment
 * variable GREAT_PROBABILITY. If this is not set, the probability
 * defaults to 0.5.
 */
int
great_random_success(void);

/*
 * Randomly return a value in the range 0 to max - 1 inclusive.
 */
unsigned int
great_random_choice(unsigned int range);

#endif

