/*
 * Shared wrapping support routines.
 *
 * $Id$
 */

#include <dlfcn.h>

#include "wrap.h"

void *
greap_wrap_resolve(const char *functionname)
{
	void *f;

	/*
	 * POSIX guarentees that a function pointer is expressable by void *.
	 */
	f = dlsym(RTLD_NEXT, functionname);
	if(!f) {
		perror(dlerror());
		abort();
	}

	return f;
}

