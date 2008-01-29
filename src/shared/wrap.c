/*
 * Shared wrapping support routines.
 *
 * $Id$
 */

#include <dlfcn.h>

#include "wrap.h"

void *
wrap_resolvefunction(const char *functionname)
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

