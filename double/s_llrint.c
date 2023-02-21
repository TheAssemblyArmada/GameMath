#include "math_private.h"

#ifdef USE_SSE
#if (defined _M_IX86 || defined __i386__)
#define USE_32BIT
#endif

long long gm_llrint(double x)
{
#ifdef USE_32BIT
    return _mm_cvtsd_si32(_mm_load_sd(&x));
#else
    return _mm_cvtsd_si64(_mm_load_sd(&x));
#endif
}
#else
#define type		double
#define	roundit		gm_rint
#define dtype		long long
#define	fn		gm_llrint

#include "s_rint.h"
#endif /* USE_SSE */
