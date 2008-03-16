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

#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>

#include "log.h"
#include "subset.h"
#include "../port/time.h"

int fd;
const char *libname;
const char *stdname;

static void
writeint(int i, int base, const char digits[])
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

static void
vlog(enum great_log_level level, const char *facility, const char *section, const char *fmt, va_list ap) 
{
	char buf[26];

	assert(facility);
	assert(libname);

	great_subset_disable();

	great_timestamp(buf);
    /* -2 to cut off the \n\0 */
    write(fd, buf, sizeof buf - 2);
	write(fd, " ", 1);
	write(fd, libname, strlen(libname));
	write(fd, " ", 1);
	write(fd, facility, strlen(facility));
	write(fd, " ", 1);
	if (stdname && section) {
		assert(strlen(stdname) > 0);
		assert(strlen(section) > 0);

		write(fd, "[", 1);
		write(fd, stdname, strlen(stdname));
		write(fd, " ", 1);
		write(fd, section, strlen(section));
		write(fd, "] ", 2);
	}

	switch (level) {
	case GREAT_LOG_INFO:
		write(fd, "INFO", strlen("INFO"));
		break;

	case GREAT_LOG_DEBUG:
		write(fd, "DEBUG", strlen("DEBUG"));
		break;

	case GREAT_LOG_INTERCEPT:
		write(fd, "IB", strlen("IB"));
		break;

	case GREAT_LOG_DEFAULT:
		write(fd, "DEFAULT", strlen("DEFAULT"));
		break;

	case GREAT_LOG_ERROR:
		write(fd, "ERROR", strlen("ERROR"));
		break;

	case GREAT_LOG_UNDEFINED:
		write(fd, "UB", strlen("UB"));
		break;
	}

	if (fmt) {
		write(fd, ": ", 2);

		vlogf(fmt, ap);
	}

	write(fd, "\n", 1);

	great_subset_enable();
}

void
great_log_init(const char *name, const char *standard)
{
	const char *logfile;
	int f;

	assert(name);

	libname = name;
	stdname = standard;

	/* default to stderr */
	fd = STDERR_FILENO;

	logfile = getenv("GREAT_LOG");
	if (!logfile) {
		return;
	}

	if (0 == strlen(logfile)) {
		return;
	}

	if (!strcmp(logfile, "-")) {
		fd = STDOUT_FILENO;
		return;
	}

	f = open(logfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

