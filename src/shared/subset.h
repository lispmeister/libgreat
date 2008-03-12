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
 * This provides the underlying support for selecting which subsets of an API
 * the library may override with its own behaviour.
 *
 * Subsets may be selected by way of regular expressions. These are given via
 * the environment variable $GREAT_SUBSETS, which may either contain a single
 * regex, or a list delimited by non-special punctuation characters.
 *
 * If not set, $GREAT_SUBSETS defaults to '.', thereby matching all functions.
 *
 * API names are categorised into headers, groups and function names. These
 * are colon-separated - for example, malloc() is "stdlib:memory:malloc". This
 * is the form matched against the given regular expressions.
 *
 * The group names are essentially arbitrary, but are intended to correspond to
 * sections within standards.
 *
 * Functions may call great_subset() to find if they have been selected.
 *
 * This mechanism has no concept of to which standard a function belongs, as
 * that is implicit from the library being linked.
 *
 *
 * The system cannot operate with per-function granularity in all cases. For
 * example, overriding rand() also requires srand() to be overridden, else their
 * behaviours would become inconsistent. Hence for these co-dependant functions,
 * enabling one also has the effect of enabling the other, and vice-versa. The
 * idiom for programatically expressing this co-dependence is:
 *
 *	if (!great_subset("stdlib:prng:rand")
 *	&& !great_subset("stdlib:prng:srand")) {
 *		return great_c99.rand();
 *	}
 *
 * This reads as "overriding rand() depends on both stdlib:prng:rand and
 * stdlib:prng:srand not being matched". In other words, if either are,
 * matched then we must override rand().
 *
 * In another example, we can override (for example) realloc without depending
 * on malloc. However, our free() depends on both. The exact behaviour here is
 * specific to how such functions are implemented.
 *
 *
 * Examples of use:
 *
 *	$GREAT_SUBSETS='^stdlib:prng:'
 *	$GREAT_SUBSETS='/stdlib:prng/stdlib:memory/'
 *	$GREAT_SUBSETS=':rand$'
 *	$GREAT_SUBSETS='/^stdlib:/^stdio:/^stdbool:'
 *	$GREAT_SUBSETS=
 *
 * The latter disables all overrides, producing the system default.
 *
 * $Id$
 */

#ifndef GREAT_SHARED_SUBSET_H
#define GREAT_SHARED_SUBSET_H

#include <stdbool.h>

/*
 * Initialise the subset system. This causes the set of subsets to be populated
 * from the environment variable $GREAT_SUBSETS, which is not used after this
 * call.
 */
bool great_subset_init(void);

/*
 * Find if a given API function is matched by $GREAT_SUBSETS.
 *
 * Functions are expected to be given in the form "header:group:function".
 * For example: great_subset("stdlib:memory:malloc")
 */
bool great_subset(const char *name);

/*
 * Temporarily disable all subsets. In conjunction with great_subset_enable(),
 * this is intended to provide a "wrap-free" region of code for internal use.
 */
void great_subset_disable(void);

/*
 * Reenable subssets previously disabled by great_subset_disable().
 */
void great_subset_enable(void);

#endif

