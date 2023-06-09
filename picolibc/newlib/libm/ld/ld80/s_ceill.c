/* @(#)s_ceil.c 5.1 93/09/24 */
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
 * ceill(x)
 * Return x rounded toward -inf to integral value
 * Method:
 *	Bit twiddling.
 * Exception:
 *	Inexact flag raised if x not equal to ceil(x).
 */



static const long double huge = 1.0e4930L;

long double
ceill(long double x)
{
	int32_t i1,jj0;
	u_int32_t i,j,se,i0,sx;
	GET_LDOUBLE_WORDS(se,i0,i1,x);
	sx = (se>>15)&1;
	jj0 = (se&0x7fff)-0x3fff;
	if(jj0<31) {
	    if(jj0<0) {	/* raise inexact if x != 0 */
		if(huge+x>0.0L) {/* return 0*sign(x) if |x|<1 */
		    if(sx) {se=0x8000;i0=0;i1=0;}
		    else if((i0|i1)!=0) { se=0x3fff;i0=0x80000000L;i1=0;}
		}
	    } else {
		i = (0x7fffffff)>>jj0;
		if(((i0&i)|i1)==0) return x; /* x is integral */
		if(huge+x>0.0L) {	/* raise inexact flag */
		    if(sx==0) {
			if (jj0>0 && (i0+(0x80000000>>jj0))>i0)
			  i0+=0x80000000>>jj0;
			else
			  {
			    i = 0x7fffffff;
			    ++se;
			  }
		    }
		    i0 &= (~i); i1=0;
		}
	    }
	} else if (jj0>62) {
	    if(jj0==0x4000) return x+x;	/* inf or NaN */
	    else return x;		/* x is integral */
	} else {
	    i = ((u_int32_t)(0xffffffff))>>(jj0-31);
	    if((i1&i)==0) return x;	/* x is integral */
	    if(huge+x>0.0L) {		/* raise inexact flag */
		if(sx==0) {
		    if(jj0==31) i0+=1;
		    else {
			j = i1 + (1<<(63-jj0));
			if(j<(u_int32_t)i1) i0+=1;	/* got a carry */
			i1 = j;
		    }
		}
		i1 &= (~i);
	    }
	}
	SET_LDOUBLE_WORDS(x,se,i0,i1);
	return x;
}
