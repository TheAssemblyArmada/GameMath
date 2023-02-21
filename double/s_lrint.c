#include "math_private.h"

#ifdef USE_SSE
#if (defined _M_IX86 || defined __i386__)
#define USE_32BIT
#endif

long gm_lrint(double x)
{
#ifdef USE_32BIT
    return (long)_mm_cvtsd_si32(_mm_load_sd(&x));
#else
    return (long)_mm_cvtsd_si64(_mm_load_sd(&x));
#endif
}
#else
#ifndef type
#define type		double
#define	roundit		gm_rint
#define dtype		long
#define	fn		gm_lrint
#endif

#include "s_rint.h"
#endif /* USE_SSE */
