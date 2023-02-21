#include "math_private.h"

#ifdef USE_SSE
#if (defined _M_IX86 || defined __i386__)
#define USE_32BIT
#endif

long long gm_llrintf(float x)
{
#ifdef USE_32BIT
    return _mm_cvtss_si32(_mm_load_ss(&x));
#else
    return _mm_cvtss_si64(_mm_load_ss(&x));
#endif
}
#else
#define type		float
#define	roundit		gm_rintf
#define dtype		long long
#define	fn		gm_llrintf

#include "s_rint.h"
#endif /* USE_SSE */
