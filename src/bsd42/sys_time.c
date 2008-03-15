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
 * BSD 4.2 <sys/time.h>
 *
 * $Id$
 */

#include <stddef.h>
#include <sys/time.h>

#include "wrap.h"
#include "../shared/subset.h"
#include "../shared/random.h"
#include "../shared/log.h"

int
gettimeofday(struct timeval * restrict tp, void * restrict tzp)
{
	if (!great_subset("sys:time:gettimeofday")) {
		return great_bsd42.gettimeofday(tp, tzp);
	}

	if (!great_random_bool(NULL)) {
		great_log(GREAT_LOG_DEFAULT, "sys:time:gettimeofday", NULL);
		return great_bsd42.gettimeofday(tp, tzp);
	}

	if (!tp) {
		great_ib("sys:time:gettimeofday", "gettimeofday(3)", "Returning success");

		return 0;
	}

	/*
	 * gettimeofday(3): "The system's notion of the current UTC time ..."
	 *
	 * Our notion is that the time keeps changing randomly.
	 */
	tp->tv_sec = great_random_long(NULL);
	tp->tv_usec = great_random_long(NULL);

	great_ib("sys:time:gettimeofday", "gettimeofday(3)", "Returning random time");

	return 0;
}

