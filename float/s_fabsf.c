/* s_fabsf.c -- float version of s_fabs.c.
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

/*
 * fabsf(x) returns the absolute value of x.
 */

#include "gmath.h"
#include "math_private.h"

float
gm_fabsf(float x)
{
#if defined USE_SSE && (defined __amd64__ || defined _M_AMD64)
	// Testing shows this only improves performance on x86_64 builds
	float ret;
	const union {
		unsigned __ui;
		float __uf;
	} sign_mask = { 0x7FFFFFFF };

	_mm_store_ss(&ret, _mm_and_ps(_mm_load_ss(&sign_mask.__uf), _mm_load_ss(&x)));
	return ret;
#else
	uint32_t ix;
	GET_FLOAT_WORD(ix,x);
	SET_FLOAT_WORD(x,ix&0x7fffffff);
        return x;
#endif
}
