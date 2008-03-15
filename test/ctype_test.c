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

/* Required for isblank() on GNU systems */
#define _ISOC99_SOURCE

#include <ctype.h>
#include <stdio.h>

static void
testc(const char *s, const char *fn, int (*xis)(int c))
{
	printf("%8s", fn);

	for ( ; *s; s++) {
		printf("%4x", xis(*s));
	}

	putc('\n', stdout);
}

static void
testh(const char *s)
{
	printf("%8s", "");
	for ( ; *s; s++) {
		printf(isprint((int) *s) ? "%4c" : "%#4x", *s);
	}
	putc('\n', stdout);
}

int
main(int argc, char *argv[])
{
	if (2 != argc) {
		return 1;
	}

	testh(argv[1]);
	testc(argv[1], "isalnum", isalnum);
	testc(argv[1], "isalpha", isalpha);
	testc(argv[1], "isblank", isblank);
	testc(argv[1], "iscntrl", iscntrl);
	testc(argv[1], "isdigit", isdigit);
	testc(argv[1], "isgraph", isgraph);
	testc(argv[1], "islower", islower);
	testc(argv[1], "isprint", isprint);
	testc(argv[1], "ispunct", ispunct);
	testc(argv[1], "isspace", isspace);
	testc(argv[1], "isupper", isupper);
	testc(argv[1], "isxdigit", isxdigit);

	return 0;
}

