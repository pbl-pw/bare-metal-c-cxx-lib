/*
  (c) Copyright 2019 Joel Sherrill <joel@rtems.org
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <fenv.h>
#include <errno.h>

/*
FUNCTION
<<fesetexceptflag>>---set floating-point status flags

INDEX
	fesetexceptflag

SYNOPSIS
	#include <fenv.h>
	int fesetexceptflag(const fexcept_t *<[flagp]>, int <[excepts]>);

	Link with -lm.

DESCRIPTION
This method attempts to set the floating-point status flags specified
by <[excepts]> to the states indicated by <[flagp]>.  The argument
<[flagp]> must point to an fexcept_t instance obtained via calling
<<fegetexceptflag>> with at least the floating-point exceptions specified
by the argument <[excepts]>.

This method does not raise any floating-point exceptions. It only
sets the state of the flags.

RETURNS
If the information was successfully returned, this method returns
zero. Otherwise, a non-zero value is returned.

PORTABILITY
ANSI C requires <<fesetexceptflag>>.

Not all Newlib targets have a working implementation.  Refer to
the file <<sys/fenv.h>> to see the status for your target.
*/

/*
 * This is a non-functional implementation that should be overridden
 * by an architecture specific implementation in newlib/libm/machine/ARCH.
 */
int fesetexceptflag(const fexcept_t *flagp, int excepts)
{
    (void) excepts;
    if (*flagp == 0)
        return 0;
    return -ENOTSUP;
}
