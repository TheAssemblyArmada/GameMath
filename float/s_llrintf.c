#include "math_private.h"

#if defined(USE_SSE) && !(defined _M_IX86 || defined __i386__)
long long gm_llrintf(float x)
{
    return _mm_cvtss_si64(_mm_load_ss(&x));
}
#else
#define type		float
#define	roundit		gm_rintf
#define dtype		long long
#define	fn		gm_llrintf

#include "s_rint.h"
#endif /* USE_SSE */
