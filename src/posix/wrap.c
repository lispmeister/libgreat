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
 * Shared wrapping support routines.
 *
 * $Id$
 */

/* Required for RTLD_NEXT on GNU systems */
#define _GNU_SOURCE

#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>

#include "../wrap.h"
#include "../shared/log.h"

void (*
great_wrap_resolve(const char *functionname))(void)
{
	void (**f)(void);

	assert(functionname);

	/*
	 * POSIX guarantees that a function pointer is expressible by void *.
	 * Rather than conform to this assumption (which makes GCC unhappy, and
	 * understandably so), this is avoided by resolving a pointer to a pointer
	 * to a function, and then dereferencing that.
	 *
	 * This works because the value of a function is its address (hence
	 * dereferencing a pointer to a function gives the address of the function
	 * again). C defines that any function pointer may express the address of
	 * any other function pointer, hence for void (**f)(void), *f becomes our
	 * equivalent to void *.
	 */

	f = dlsym(RTLD_NEXT, functionname);
	if(!f) {
		great_log(GREAT_LOG_ERROR, "wrap", "dlsym: %s", dlerror());
		abort();
	}

	return *f;
}

