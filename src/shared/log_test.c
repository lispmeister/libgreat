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
 * $Id$
 */

#include <string.h>
#include <stddef.h>
#include <errno.h>

#include "log.h"

int
main(void)
{
	int a[] = { 0, 1, 7, 9, 10, 11, 15, 16, 17, 123, 99972, -1, -2, -37, -937 };
	size_t i;

	great_log_init("logtest", "LT");

	errno = EPERM;
	great_perror("x", "fopen");

	great_ub("xyz", "1.2.3", "a %s c", "b");
	great_ib("xyz", "4.5.6", "x %s z", "y");

	great_log(GREAT_LOG_DEFAULT, "stdio:prng:rand", "abc %s", "def");
	great_log(GREAT_LOG_DEBUG, "x", "%%");
	great_log(GREAT_LOG_UNDEFINED, "x", NULL);
	great_log(GREAT_LOG_ERROR, "y", "%s", strerror(ENOENT));
	great_log(GREAT_LOG_INTERCEPT, "stdio:prng:rand", "%c%s%c%s", 'a', "bc", 'd', "efg");
	for (i = 0; i < sizeof a / sizeof *a; i++) {
		great_log(GREAT_LOG_INFO, "i", "%o %d %i %x %X",
			a[i], a[i], a[i], a[i], a[i]);
	}

	great_log_fini();

	return 0;
}

