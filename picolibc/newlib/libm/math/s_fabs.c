
/* @(#)s_fabs.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
FUNCTION
       <<fabs>>, <<fabsf>>---absolute value (magnitude)
INDEX
	fabs
INDEX
	fabsf

SYNOPSIS
	#include <math.h>
       double fabs(double <[x]>);
       float fabsf(float <[x]>);

DESCRIPTION
<<fabs>> and <<fabsf>> calculate
@tex
$|x|$,
@end tex
the absolute value (magnitude) of the argument <[x]>, by direct
manipulation of the bit representation of <[x]>.

RETURNS
The calculated value is returned.  No errors are detected.

PORTABILITY
<<fabs>> is ANSI.
<<fabsf>> is an extension.

*/

/*
 * fabs(x) returns the absolute value of x.
 */

#include "fdlibm.h"

#ifdef _NEED_FLOAT64

__float64
fabs64(__float64 x)
{
    __uint32_t high;
    GET_HIGH_WORD(high, x);
    SET_HIGH_WORD(x, high & 0x7fffffff);
    return x;
}

_MATH_ALIAS_d_d(fabs)

#endif /* _NEED_FLOAT64 */
