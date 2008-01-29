/*
 * Shared wrapping support routines.
 *
 * $Id$
 *
 * Our approach here is to populate our own list of function pointers
 * during initialisation. These may then be referenced by our own functions.
 *
 * These lists are maintained in each API's implementation - see <api>/wrap.h.
 * They are populated during each API's _init() routine. If they fail to
 * locate the symbols required from the underlying libraries, they are expected
 * to abort().
 *
 * Only the functions overloaded are expected to be wrapped.
 */

#ifndef GREAT_SHARED_WRAP_H
#define GREAT_SHARED_WRAP_H

/*
 * dlsym() the given function name.
 *
 * If it fails to locate the symbol from RTLD_NEXT, it will abort().
 */
void *
wrap_resolvefunction(const char *functionname);

#endif

