# Options specific to GCC.
# These warnings were mostly collated by Saparok from ##c;
# see http://www.iso-9899.info/wiki/WarningFlags for details.
#
# $Id$

CFLAGS +=
SFLAGS += -fPIC

# Partial makefile example with majority of gcc warnings
#
# Only a few of the code generation, optimization or architecture options
# are shown here.  See the gcc documentation for details.

# Optimizer settings
# CFLAGS += -O      # equivalent to -O1
CFLAGS += -O2       # most common optimization setting
# CFLAGS += -O3     # turn on -finline-functions, -funswitch-loops, etc.
# CFLAGS += -O0     # disable optimization (default)
# CFLAGS += -fast   # optimize for maximum performance
# CFLAGS += -Os     # optimize for size at the expense of speed.
# CFLAGS += -Oz     # Apple OS X only size optimization setting.

# Debugging
# Produce debugging information in the native platform debug format.
# CFLAGS += -g

# Version of C desired
# CFLAGS += -std=c89     # Really means ISO C90
# CFLAGS += -ansi        # equivalent to -std=c89
CFLAGS += -std=c99       # Only /partial/ C99 support.  See gnu.org for status.
# CFLAGS += -std=gnu89   # ISO C90 plus GNU extensions and some C99 features
# CFLAGS += -std=gnu99   # ISO C99 plus GNU extensions

# Warn about non-ISO looking code, stricter checking for conformance testing
# with one of the ANSI/ISO std options above.  Issue all warnings demanded by
# strict ISO C for the appropriate standard.  Turns off some gcc extensions
# that are left on without it.
CFLAGS += -pedantic
# CFLAGS += -pedantic-errors   # like -pedantic, but errors instead of warnings

# Misnamed, enables a list of warning options, but not all of them.
CFLAGS += -Wall

# Warn if an array subscript has type "char" (which can be signed)
# CFLAGS += -Wchar-subscripts    # included in -Wall

# Warn if a comment sequence /* appears in another /* comment or a
# backslash-newline appears in a // comment.
# CFLAGS += -Wcomment            # included in -Wall

# Check calls to printf, scanf, etc to make sure args have proper types.
# CFLAGS += -Wformat             # included in -Wall
# CFLAGS += -Wnonnull            # included in -Wall
# CFLAGS += -Wimplicit-int       # included in -Wall
# CFLAGS += -Wimplicit-function-declaration  # included in -Wall
# CFLAGS += -Wimplicit           # included in -Wall
# CFLAGS += -Wmain               # included in -Wall
# CFLAGS += -Wmissing-braces     # included in -Wall
# CFLAGS += -Wparentheses        # included in -Wall
# CFLAGS += -Wsequence-point     # included in -Wall
# CFLAGS += -Wreturn-type        # included in -Wall
# CFLAGS += -Wswitch             # included in -Wall
# CFLAGS += -Wtrigraphs          # included in -Wall
# CFLAGS += -Wunused-function    # included in -Wall
# CFLAGS += -Wunused-label       # included in -Wall
# CFLAGS += -Wunused-variable    # included in -Wall
# CFLAGS += -Wunused-value       # included in -Wall
# CFLAGS += -Wuninitialized      # included in -Wall
# CFLAGS += -Wstrict-aliasing    # included in -Wall if -fstrict-aliasing

# Warn if a user-supplied include directory does not exist.
# CFLAGS += -Wmissing-incude-dirs

# Warn whenever a switch statement does not have a default case.
# Unfortunately this includes switching on an enum where all cases
# are covered.
# CFLAGS += -Wswitch-default
# Warn when a switch statement has an index of enumerate type and lacks a case
# for one or more of the named codes in the enumeration, or case labels outside
# the enumeration range.
CFLAGS += -Wswitch-enum

# Warn whenever a function parameter is unused aside from its declaration,
# unless the "unused" attribute is used.
# CFLAGS += -Wunused-parameter

# warn when an unknown #pragma directive is encountered.
# CFLAGS += -Wunknown-pragmas

# Print extra warning information.  This is equivalent to -W, may need to use
#    -W with older versions of gcc
# CFLAGS += -Wextra
CFLAGS += -W
# Warn for incomplete struct initialisations, other than designated initializers
# CFLAGS += -Wmissing-field-initializers  # included in -Wextra

# Warn about comparison between signed and unsigned values that can produce
# an incorrect result when the signed value is converted to unsigned.
# CFLAGS += -Wsign-compare    # in -Wextra, without -Wno-sign-compare

# Warnings are errors.
CFLAGS += -Werror
# abort compilation on first error rather than continuing
# CFLAGS += -Wfatal-errors

# warn about constructs that behave differently in traditional and ISO C, or
# those with no traditional C equivalent, or problematic ones to be avoided.
# CFLAGS += -Wtraditional

# Warn for missing prototypes.
CFLAGS += -Wmissing-prototypes

# Warn if a function is declared or defined w/o specifying the argument types.
CFLAGS += -Wstrict-prototypes

# Warn for a global function defined without a previous prototype.  Issued even
# if the definition itself provides a prototype.
CFLAGS += -Wmissing-declarations

# Warn when a variable is shadowed, or a built-in function is shadowed.
CFLAGS += -Wshadow

# Warn when an object larger than *len* bytes is defined.
# CFLAGS += -Wlarger-than-len

# Allocate even uninitialized global variables in the data section of the
# object file, rather than generating them as common blocks.  If the same
# variable is declared without "extern" in two different compilations, you
# will get an error when linked.
CFLAGS += -fno-common

# Do not store floating point variables in registers and inhibit other options
# that might change whether a floating point value is taken from a register or
# memory.  Useful primarily for programs that rely in a precise definition of
# IEEE floating point.
# CFLAGS += -ffloat-store

# Turns on -fno-math-errno, -funsafe-math-optimizations, -fno-trapping-math,
# -ffinite-math-only, -fno-rounding-math, -fno-signaling-nans and
# fcx-limited-range.  Should never be turned on with optimizations, since it
# can result in incorrect output for programs that depend upon an exact
# implementation of IEEE or ISO math function behavior.
# CFLAGS += -ffast-math

# Warn if any function that return structures or unions are defined or called.
CFLAGS += -Waggregate-return

# Targetted primarily for interfacing with code that doesn't use prototypes;
# otherwise pretty noisy.  Also warn about "x = -1;" if x is unsigned.
# CFLAGS += -Wconversion

# Warn for floating point equality expressions.
CFLAGS += -Wfloat-equal

# Warn for long long usage.  (only if -pedantic is used)
# CFLAGS += -Wlong-long

# Warn for multiple declarations for the same thing in the same scope.
CFLAGS += -Wredundant-decls

# Warn for function call cast to non-matching type.
CFLAGS += -Wbad-function-cast

# Warn when a pointer is cast to remove a type qualifier, ex: warn if
# "const char *" is cast to an ordinary "char *".
CFLAGS += -Wcast-qual

# Warn for casts which increase alignment constraints, such as "char *" cast
# to "int *" on machines where unaligned accesses are not alowed.
CFLAGS += -Wcast-align

# Make string literals const.
CFLAGS += -Wwrite-strings

# Warn for undefined identifiers in #if directives.
CFLAGS += -Wundef

# Warn for text at end of preprocessor directive.
CFLAGS += -Wendif-labels

# Don't warn about unused function parameters.
CFLAGS += -Wno-unused-parameter

# Warn about anything that depends on the size of a function type or a "void".
CFLAGS += -Wpointer-arith

# Warn if function declared as inline can not be inlined.
CFLAGS += -Winline

# Warn if a requested optimization pass is disabled, meaning the gcc optimizer
# is not able to handle the code.  The code may be too big or complex.
CFLAGS += -Wdisabled-optimization

# CFLAGS += -Wdeclaration-after-statement  # after a statement in a block, i.e C99

# warn if a 64-bit value is implicitly converted from a 64- to a 32-bit type.
# CFLAGS += -Wshorten-64-to-32

# Warn if an old-style function definition is used, even with a valid prototype.
# CFLAGS += -Wold-style-definition

# Warn about functions which might be candidates for attribute "noreturn".
CFLAGS += -Wmissing-noreturn

# warn of user-supplied include dirs that are missing
# CFLAGS += -Wmissing-include-dirs

# warn about strftime formats which yield a 2-digit year
CFLAGS += -Wformat-y2k    # if -Wformat is also specified only

# if -Wformat is enabled, warn if the format string is not a string literal and
# cannot be checked, unless it takes its format arguments as a "va_list"
CFLAGS += -Wformat-nonliteral # format string can not be checked

# format security problems get flagged, like printf(foo);
CFLAGS += -Wformat-security

# Warn the first time #import is used.
CFLAGS += -Wimport

# Warn about uninitialized variables that are initialized with themselves.
# Examples: int i = i;
# Requires -Wuninitialized and -O1 or above.
# CFLAGS += -Winit-self

# warn if precompiled header is found but can't be used
# CFLAGS += -Winvalid-pch

CFLAGS += -Wpacked # warn if packed attibute has no effect

CFLAGS += -Wpadded  # warn if padding is included in a structure

# This will cause assert() to emit a warning, even when NDEBUG is not defined.
# CFLAGS += -Wunreachable-code

# Disable "asm", "inline" or "typeof" as keywords.  -ansi implies this option.
# With C99, "inline" is still allowed.
# CFLAGS += -fno-asm

# Don't use special code for certain "built in" functions.  For use of some
# LD_PRELOAD tricks this might be necessary.
# CFLAGS += -fno-builtin

# Warn if an "extern" declaration is encountered within a function.
CFLAGS += -Wnested-externs

# Warn about four character constants, 'APPL'.
# CFLAGS += -Wfour-char-constants
# CFLAGS += -Wno-multichar   # do not warn if a multichar constant 'FOO' is used

# ISO C identifier normalization options.  See man page.
# CFLAGS += -Wnormalized=<none|id|nfc|nfkc>

# Generate code to verify you do not go beyond the stack boundary.  Useful
# primarily for threaded (PTHREAD) code.  This does /not/ cause stack checking
# to be done; the operating system must do that.  Generates code to ensure that
# the OS sees the stack being extended.
# This causes the compiler to write a byte to each page of the local variable
# area on entry to each function, thus removing any chance that the overflow will
# miss the guard page.  Has been reported buggy in some implementations,
# particularly with stack usage inside main().
# See: http://www.kegel.com/stackcheck/demo.c
# CFLAGS += -fstack-check
# See also -fstack-limit-symbol, -fstack-limit-register

# CFLAGS += -fstack-protector  # required for the -Wstack-protector
# Warn about functions that will not be protected against stack smashing
# CFLAGS += -Wstack-protector

