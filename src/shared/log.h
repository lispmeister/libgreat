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
 * This provides a facility by which log messages may be given to provide some
 * insight into the operations of the library. It is provided for two reasons:
 * firstly to give a consistent and centralised interface, based on the user's
 * configuration.
 *
 * Secondly, it avoids using functions internally which may be affected by the
 * interception provided for by the library itself (for example, if printf()
 * were to call malloc() internally, and malloc() were to be intercepted to
 * fail. Other than failure conditions, this also avoids infinite recursion.
 *
 * $Id$
 */

#ifndef GREAT_SHARED_LOG_H
#define GREAT_SHARED_LOG_H

/*
 * Logging levels represent the severity of a given log message, grouped
 * roughly according to origin. These are as follows:
 *
 *	INFO		Informational messages on normal operations
 * 	DEBUG		For development use
 *	INTERCEPT	Interceptions against default behaviours
 *	DEFAULT		Fallbacks to default behaviours
 *	ERROR		User-facing errors conditions
 */
enum great_log_level {
	GREAT_LOG_INFO,
	GREAT_LOG_DEBUG,
	GREAT_LOG_INTERCEPT,
	GREAT_LOG_DEFAULT,
	GREAT_LOG_ERROR
};

/*
 * Initialise logging. This must be called before use.
 *
 * The name passed is taken as the name of the library; this is used to
 * prefix log messages.
 *
 * The file to which logs are written is given by the environment variable
 * GREAT_LOG. This may may be a filename, or "-" to indicate stdout. If not
 * set or empty, this defaults to stderr.
 */
void
great_log_init(const char *name);

void
great_log_fini(void);

/*
 * This is a formatted logging string in the style of printf. This is intended
 * for internal use only, and hence assert()s on various error conditions.
 *
 * Logs are formatted as "timestamp facility level: msg", where the timestamp
 * is given by asctime().
 *
 * The file written to is set by $GREAT_LOG; see great_log_init() for details.
 *
 * Logging prefixes a timestamp of the time at which the message was generated,
 * and the name of the library as given to great_log_init().
 *
 * The formats provided are:
 *
 *	%%		A literal %
 *	%s 		A string given by char * formatted byte-by-byte
 *	%c		A char formatted as a byte
 *	%o		An int formatted as octal
 *	%i, %d	An int formatted as decimal
 *	%x, %X	An int formatted as hexadecimal in lower and upper case respectively
 *
 * Other characters are passed through as-is. These must be printable
 * characters, as defined by isprint().
 *
 * No conversion specifiers, lengths, widths, precisions or flags are provided.
 */
void
great_log(enum great_log_level level, const char *facility, const char *fmt, ...);

/*
 * This is an analogue of perror(), provided for convenience. It serves to call
 * great_log() for GREAT_LOG_ERROR, taking its message from strerror(errno).
 */
void
great_perror(const char *name, const char *string);

#endif

