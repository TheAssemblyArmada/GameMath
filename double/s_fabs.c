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
 * fabs(x) returns the absolute value of x.
 */

#include "gmath.h"
#include "math_private.h"

double
gm_fabs(double x)
{
#if defined USE_SSE && (defined __amd64__ || defined _M_AMD64)
	// Testing shows this only improves performance on x86_64 builds
	double ret;
	const union {
		unsigned long long __ui;
		double __uf;
	} sign_mask = { 0x7FFFFFFFFFFFFFFF };

	_mm_store_sd(&ret, _mm_and_pd(_mm_load_sd(&sign_mask.__uf), _mm_load_sd(&x)));
	return ret;
#else
	uint32_t high;
	GET_HIGH_WORD(high,x);
	SET_HIGH_WORD(x,high&0x7fffffff);
        return x;
#endif
}
