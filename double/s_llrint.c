#include "math_private.h"

#if defined(USE_SSE) && !(defined _M_IX86 || defined __i386__)
long long gm_llrint(double x)
{
    return _mm_cvtsd_si64(_mm_load_sd(&x));
}
#else
#define type		double
#define	roundit		gm_rint
#define dtype		long long
#define	fn		gm_llrint

#include "s_rint.h"
#endif /* USE_SSE */
