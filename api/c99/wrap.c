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

#include <stdio.h>

#include "wrap.h"
#include "../../src/wrap.h"
#include "../../src/shared/random.h"
#include "../../src/shared/subset.h"
#include "../../src/shared/log.h"

struct great_c99 great_c99;

/* TODO provide static initialisation alternative */
void
_init(void) {
	/* ctype.c */
	great_c99.isalnum =
		(int (*)(int c)) great_wrap_resolve("isalnum");
	great_c99.isalpha =
		(int (*)(int c)) great_wrap_resolve("isalpha");
	great_c99.isblank =
		(int (*)(int c)) great_wrap_resolve("isblank");
	great_c99.iscntrl =
		(int (*)(int c)) great_wrap_resolve("iscntrl");
	great_c99.isdigit =
		(int (*)(int c)) great_wrap_resolve("isdigit");
	great_c99.isgraph =
		(int (*)(int c)) great_wrap_resolve("isgraph");
	great_c99.islower =
		(int (*)(int c)) great_wrap_resolve("islower");
	great_c99.isprint =
		(int (*)(int c)) great_wrap_resolve("isprint");
	great_c99.ispunct =
		(int (*)(int c)) great_wrap_resolve("ispunct");
	great_c99.isspace =
		(int (*)(int c)) great_wrap_resolve("isspace");
	great_c99.isupper =
		(int (*)(int c)) great_wrap_resolve("isupper");
	great_c99.isxdigit =
		(int (*)(int c)) great_wrap_resolve("isxdigit");

	/* stdio_fileaccess.c */
	great_c99.fopen =
		(FILE *(*)(const char * restrict, const char * restrict))
		great_wrap_resolve("fopen");

	/* stdlib_prng.c */
	great_c99.rand =
		(int (*)(void)) great_wrap_resolve("rand");
	great_c99.srand =
		(void (*)(unsigned int)) great_wrap_resolve("srand");

	/* stdlib_memory.c */
	great_c99.free =
		(void (*)(void *)) great_wrap_resolve("free");
	great_c99.malloc  =
		(void *(*)(size_t)) great_wrap_resolve("malloc");
	great_c99.realloc =
		(void *(*)(void *, size_t)) great_wrap_resolve("realloc");

	great_subset_disable();

	/* C99 7.20.2.2 P2 If rand is called before any calls to srand have been
	 * made, the same sequence shall be generated as when srand is first called
	 * with a seed value of 1. */
	great_random_init(&great_c99.random_rand);
	great_random_seed(&great_c99.random_rand, 1);

	great_log_init("libgreat_c99", "C99");
	great_random_init(NULL);
	great_subset_init();

	great_subset_enable();
}

