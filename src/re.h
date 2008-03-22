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
 * Regular expression interfaces.
 *
 * $Id$
 */

#ifndef GREAT_PORT_RE_H
#define GREAT_PORT_RE_H

#include <stddef.h>
#include <stdbool.h>


struct great_re;

/*
 * Given an error code supplied from great_re_comp(), write out a corresponding
 * error message in string form to buf, limited to length len (including the
 * terminating null).
 */
void
great_re_error(int err, char *buf, size_t len);

/*
 * Allocate a regular expression, compiled from the string rs.
 *
 * Returns NULL on error. If err is non-NULL, *err is assigned an error code
 * which may then be passed to great_regerror().
 */
struct great_re *
great_re_comp(const char *rs, int *err);

/*
 * Free a regex allocated by great_re_comp().
 */
void
great_re_free(struct great_re *re);

/*
 * Match a regex against a given string. Returns true if the string is matched
 * and false otherwise.
 */
bool
great_re_match(struct great_re *re, const char *s);

#endif

