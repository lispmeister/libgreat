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
 * Shared logging.
 *
 * TODO Set logging level by environment
 *
 * $Id$
 */

#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>
#include <limits.h>
#include <stdio.h>

#include "log.h"
#include "subset.h"
#include "../port/time.h"
#include "../port/io.h"

FILE *fp;
const char *libname;
const char *stdname;

/*
 * This is a buffer maintained for log messages; they are output one line at a
 * time.
 */
char *buffer;
size_t bufferindex;
size_t buffersize;

/*
 * Prerequisite: s contains one or more characters before a newline.
 */
static void
flush(char *s) {
	char *p;

	assert(s);
	assert(*s);
	assert(*s != '\n');

	p = memchr(s, '\n', bufferindex);
	if (!p) {
		return;
	}

	/* strchr() should return >= s */
	assert(p >= s);

	/* Strings should not start \n */
	assert(p > s);

	p++;	/* skip newline */

	/* All strings ought to be more than just "\n" */
	assert(p - s > 1);
	great_write(fp, s, p - s);

	assert(bufferindex >= (size_t) (p - s));
	bufferindex -= p - s;
	memmove(s, p, buffersize - bufferindex);

	if (!*s) {
		return;
	}

	flush(s);
}

/*
 * Push a string to the log buffer. If the string contains a newline,
 * everything up to and including each newline is output.
 *
 * Since this is for internal use only, it makes several demands on the
 * content of these strings, for sanity.
 *
 * Pushing a string of zero bytes is permitted (and ignored) for convenience.
 */
/* TODO consider making a variadic interface */
static void
push(const char *s, size_t len)
{
	assert(s);
	assert(*s);

	if (0 == len) {
		return;
	}

	if (len + 1 > buffersize - bufferindex) {
		size_t ns;
		char *n;

		ns = 0 == buffersize ? 128 : buffersize * 2;
		ns += len;
		assert(ns > buffersize);
		n = realloc(buffer, ns);
		if (!n) {
			/* TODO error */
			return;
		}

		buffer = n;
		buffersize = ns;
	}

	strncpy(buffer + bufferindex, s, len);
	bufferindex += len;

	flush(buffer);
}

static void
writeint(int i, int base, const char digits[])
{
	assert(base = strlen(digits));

	if (i < 0) {
		push("-", 1);
		i = abs(i);
	}

	/*
	 * do { i % 10 } while (i /= 10) is pretty, but it would give the digits
	 * in reverse order.
	 */
	if (i / base) {
		writeint(i / base, base, digits);
	}

	push(&digits[i % base], 1);
}

/*
 * Read the precision given by a formatting specifier. This is expected to be
 * passed a pointer to the character after the '.' which begins the precision
 * specificiation. It will consume as many further character as it needs, up to
 * but not including the conversion specifier.
 *
 * The argument ap is expected to point to the current va_list; this may be
 * moved along by way of va_arg() if appropiate (namely for a precision of '*').
 *
 * A pointer to the conversion specifier is returned. (That is, a pointer to
 * the next character after the precision has been dealt with.)
 */
static const char *
readprecision(const char *p, int *precision, va_list *ap)
{
	char *ep;
	long int l;

	assert(p);
	assert(*p);
	assert(precision);

	/* precision passed as parameter */
	if ('*' == *p) {
		*precision = va_arg(*ap, int);
		return p + 1;
	}

	/* No precision; e.g. "%.s" */
	if (!isdigit((unsigned char) *p)) {
		*precision = 0;
		return p;
	}

	errno = 0;
	l = strtol(p, &ep, 10);
	assert(ep > p);

	if (ERANGE == errno && (LONG_MAX == l || LONG_MIN == l)) {
		/* TODO handle error */
		return ep;
	}

	if (l > INT_MAX || l < INT_MIN) {
		/* TODO handle error */
		return ep;
	}

	*precision = l;
	return ep;
}

/*
 * Note that the format string may only contain printable characters.
 */
static void
vlogf(const char *fmt, va_list ap)
{
	const char *p;

	assert(fmt);

	for (p = fmt; *p; p++) {
		int precision = -1;

		switch (*p) {
		case '%':
			p++;
			assert(*p);

			/* readprecision() will nudge ap along for a precision of ".*" */
			if ('.' == *p) {
				p = readprecision(p + 1, &precision, &ap);
				assert(precision >= 0);
			}

			switch(*p) {
			case '%':
				push("%", 1);
				break;

			case 's': {
				char *s;

				s = va_arg(ap, char *);
				assert(s);
				push(s, -1 == precision ? (int) strlen(s) : precision);
				break;
			}

			case 'i':
			case 'd':
				writeint(va_arg(ap, int), 10, "0123456789");
				break;

			case 'o':
				writeint(va_arg(ap, int), 8, "01234567");
				break;

			case 'x':
			case 'X':
				writeint(va_arg(ap, int), 16, *p == 'x'
					? "0123456789abcdef"
					: "0123456789ABCDEF");
				break;

			case 'c': {
				char c;

				c = va_arg(ap, int);	/* promoted */
				push(&c, 1);
				break;
			}

			default:
				assert(!"Unrecognised formatting specifier");
			}

			break;

		default:
			assert(isprint((unsigned char) *p));

			push(p, 1);
			break;
		}
	}
}

static void
avlog(const char *fmt, ...)
{
	va_list ap;

	assert(fmt);

	va_start(ap, fmt);
	vlogf(fmt, ap);
	va_end(ap);
}

static void
vlog(enum great_log_level level, const char *facility, const char *section, const char *fmt, va_list ap) 
{
	char buf[26];

	assert(facility);
	assert(libname);

	great_subset_disable();

	great_timestamp(buf);
    /* -2 to cut off the \n\0 */
	avlog("%.*s %s %s ", sizeof buf - 2, buf, libname, facility);
	if (stdname && section) {
		assert(strlen(stdname) > 0);
		assert(strlen(section) > 0);

		avlog("[%s %s] ", stdname, section);
	}

	switch (level) {
	case GREAT_LOG_INFO:
		push("INFO", strlen("INFO"));
		break;

	case GREAT_LOG_DEBUG:
		push("DEBUG", strlen("DEBUG"));
		break;

	case GREAT_LOG_INTERCEPT:
		push("IB", strlen("IB"));
		break;

	case GREAT_LOG_DEFAULT:
		push("DEFAULT", strlen("DEFAULT"));
		break;

	case GREAT_LOG_ERROR:
		push("ERROR", strlen("ERROR"));
		break;

	case GREAT_LOG_UNDEFINED:
		push("UB", strlen("UB"));
		break;
	}

	if (fmt) {
		push(": ", 2);

		vlogf(fmt, ap);
	}

	push("\n", 1);

	great_subset_enable();
}

void
great_log_init(const char *name, const char *standard)
{
	const char *logfile;
	FILE *f;

	assert(name);

	libname = name;
	stdname = standard;

	/* default to stderr */
	fp = stderr;

	logfile = getenv("GREAT_LOG");
	if (!logfile) {
		return;
	}

	if (0 == strlen(logfile)) {
		return;
	}

	if (!strcmp(logfile, "-")) {
		fp = stdout;
		return;
	}

	f = fopen(logfile, "a");
	if (!f) {
		/* TODO deal with errno */
		return;
	}

	fp = f;
}

void
great_log_fini(void)
{
	if (stderr == fp) {
		return;
	}

	if (stdout == fp) {
		return;
	}

	fclose(fp);
}

void
great_log(enum great_log_level level, const char *facility, const char *fmt, ...)
{
	va_list ap;

	assert(facility);
	assert(libname);

	va_start(ap, fmt);
	vlog(level, facility, NULL, fmt, ap);
	va_end(ap);
}

void
great_perror(const char *facility, const char *string)
{
	assert(facility);
	assert(string);

	great_log(GREAT_LOG_ERROR, facility, "%s: %s", string, strerror(errno));
}

void
great_ub(const char *facility, const char *section, const char *fmt, ...)
{
	va_list ap;

	assert(facility);
	assert(section);

	va_start(ap, fmt);
	vlog(GREAT_LOG_UNDEFINED, facility, section, fmt, ap);
	va_end(ap);
}

void
great_ib(const char *facility, const char *section, const char *fmt, ...)
{
	va_list ap;

	assert(facility);
	assert(section);

	va_start(ap, fmt);
	vlog(GREAT_LOG_INTERCEPT, facility, section, fmt, ap);
	va_end(ap);
}

