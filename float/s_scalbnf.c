/* s_scalbnf.c -- float version of s_scalbn.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

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

#include "gmath.h"
#include "math_private.h"

static const float
two25   =  3.355443200e+07f,	/* 0x4c000000 */
twom25  =  2.9802322388e-08f,	/* 0x33000000 */
huge   = 1.0e+30f,
tiny   = 1.0e-30f;

float
gm_scalbnf (float x, int n)
{
	int32_t k,ix;
	GET_FLOAT_WORD(ix,x);
        k = (ix&0x7f800000)>>23;		/* extract exponent */
        if (k==0) {				/* 0 or subnormal x */
            if ((ix&0x7fffffff)==0) return x; /* +-0 */
	    x *= two25;
	    GET_FLOAT_WORD(ix,x);
	    k = ((ix&0x7f800000)>>23) - 25;
            if (n< -50000) return tiny*x; 	/*underflow*/
	    }
        if (k==0xff) return x+x;		/* NaN or Inf */
        k = k+n;
        if (k >  0xfe) return huge*gm_copysignf(huge,x); /* overflow  */
        if (k > 0) 				/* normal result */
	    {SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23)); return x;}
        if (k <= -25) {
            if (n > 50000) 	/* in case integer overflow in n+k */
		return huge*gm_copysignf(huge,x);	/*overflow*/
	    else return tiny*gm_copysignf(tiny,x);	/*underflow*/
        }
        k += 25;				/* subnormal result */
	SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23));
        return x*twom25;
}

#if defined(__clang__) || defined(__GNUC__) 
extern __typeof (gm_scalbnf)gm_ldexpf __attribute__((__alias__("gm_scalbnf")));
#elif defined(_MSC_VER)
#pragma comment(linker, "/export:_gm_ldexpf=_gm_scalbnf")
#endif
