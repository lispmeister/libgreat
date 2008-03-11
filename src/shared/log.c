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
 * TODO Write to a stack instead, and hence write() in one go.
 * TODO Set logging level by environment
 *
 * $Id$
 */

#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "log.h"

int fd;
const char *libname;

static void
timestamp(void)
{
	time_t t;
	struct tm tm;
	char buf[26];

	t = time(NULL);
	localtime_r(&t, &tm);
	asctime_r(&tm, buf);

	/* -2 to cut off the \n\0 */
	write(fd, buf, sizeof buf - 2);
}

static void
writeint(int i, unsigned int base, const char digits[])
{
	assert(base = strlen(digits));

	if (i < 0) {
		write(fd, "-", 1);
		i = abs(i);
	}

	/*
	 * do { i % 10 } while (i /= 10) is pretty, but it would give the digits
	 * in reverse order.
	 */
	if (i / base) {
		writeint(i / base, base, digits);
	}

	write(fd, &digits[i % base], 1);
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
		switch (*p) {
		case '%':
			p++;

			assert(*p);

			switch(*p) {
			case '%':
				write(fd, "%", 1);
				break;

			case 's': {
				char *s;

				s = va_arg(ap, char *);
				assert(s);
				write(fd, s, strlen(s));
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
				write(fd, &c, 1);
				break;
			}

			default:
				assert(!"Unrecognised formatting specifier");
			}

			break;

		default:
			assert(isprint((int) *p));

			write(fd, p, 1);
			break;
		}
	}
}

void
great_log_init(const char *name)
{
	const char *log;
	int f;

	/* default to stderr */
	fd = STDERR_FILENO;

	assert(name);
	libname = name;

	log = getenv("GREAT_LOG");
	if (!log) {
		return;
	}

	if (0 == strlen(log)) {
		return;
	}

	if (!strcmp(log, "-")) {
		fd = STDOUT_FILENO;
		return;
	}

	f = open(log, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (-1 == f) {
		/* TODO deal with errno */
		return;
	}

	fd = f;
}

void
great_log_fini(void)
{
	if (STDERR_FILENO == fd) {
		return;
	}

	close(fd);
}

void
great_log(enum great_log_level level, const char *name, const char *fmt, ...)
{
	va_list ap;

	assert(libname);

	timestamp();
	write(fd, " ", 1);
	write(fd, libname, strlen(libname));
	write(fd, " ", 1);

	switch (level) {
	case GREAT_LOG_INFO:
		write(fd, "INFO: ", strlen("INFO: "));
		break;

	case GREAT_LOG_DEBUG:
		write(fd, "DEBUG: ", strlen("DEBUG: "));
		break;

	case GREAT_LOG_INTERCEPT:
		write(fd, "INTERCEPT: ", strlen("INTERCEPT: "));
		break;

	case GREAT_LOG_DEFAULT:
		write(fd, "DEFAULT: ", strlen("DEFAULT: "));
		break;
	}

	va_start(ap, fmt);
	vlogf(fmt, ap);
	va_end(ap);

	write(fd, "\n", 1);
}
