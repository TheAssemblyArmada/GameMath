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
 * from: @(#)fdlibm.h 5.1 93/09/24
 * $FreeBSD$
 */

#ifndef _GMATH_H_
#define	_GMATH_H_

#ifndef __BSD_VISIBLE
#define __BSD_VISIBLE 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ANSI/POSIX
 */
extern const union __gm_infinity_un {
	unsigned char	__uc[sizeof(double)];
	double		__ud;
} __gm_infinity;

extern const union __gm_nan_un {
	unsigned char	__uc[sizeof(float)];
	float		__uf;
} __gm_nan;

#define	GM_HUGE_VAL	(__gm_infinity.__ud)

#if __ISO_C_VISIBLE >= 1999
#define	FP_ILOGB0	(-__INT_MAX)
#define	FP_ILOGBNAN	__INT_MAX

#ifdef __MATH_BUILTIN_CONSTANTS
#define	HUGE_VALF	__builtin_huge_valf()
#define	HUGE_VALL	__builtin_huge_vall()
#define	INFINITY	__builtin_inf()
#define	NAN		__builtin_nan("")
#else
#define	HUGE_VALF	(float)GM_HUGE_VAL
#define	HUGE_VALL	(long double)GM_HUGE_VAL
#define	INFINITY	HUGE_VALF
#define	NAN		(__gm_nan.__uf)
#endif /* __MATH_BUILTIN_CONSTANTS */

#define	MATH_ERRNO	1
#define	MATH_ERREXCEPT	2
#define	math_errhandling	MATH_ERREXCEPT

/* XXX We need a <machine/math.h>. */
#if defined(__ia64__) || defined(__sparc64__)
#define	FP_FAST_FMA
#endif
#ifdef __ia64__
#define	FP_FAST_FMAL
#endif
#define	FP_FAST_FMAF

/* Symbolic constants to classify floating point numbers. */
#define	FP_INFINITE	0x01
#define	FP_NAN		0x02
#define	FP_NORMAL	0x04
#define	FP_SUBNORMAL	0x08
#define	FP_ZERO		0x10
#define	gm_fpclassify(x) \
    ((sizeof (x) == sizeof (float)) ? __gm_fpclassifyf(x) \
    : (sizeof (x) == sizeof (double)) ? __gm_fpclassifyd(x) \
    : __gm_fpclassifyl(x))

#define	gm_isfinite(x)					\
    ((sizeof (x) == sizeof (float)) ? __gm_isfinitef(x)	\
    : (sizeof (x) == sizeof (double)) ? __gm_isfinite(x)	\
    : __gm_isfinitel(x))
#define	gm_isinf(x)					\
    ((sizeof (x) == sizeof (float)) ? __gm_isinff(x)	\
    : (sizeof (x) == sizeof (double)) ? gm_isinf(x)	\
    : __gm_isinfl(x))
#define	isnan(x)					\
    ((sizeof (x) == sizeof (float)) ? isnanf(x)		\
    : (sizeof (x) == sizeof (double)) ? isnan(x)	\
    : __isnanl(x))
#define	gm_isnormal(x)					\
    ((sizeof (x) == sizeof (float)) ? __isnormalf(x)	\
    : (sizeof (x) == sizeof (double)) ? __isnormal(x)	\
    : __isnormall(x))

#ifdef __MATH_BUILTIN_RELOPS
#define	isgreater(x, y)		__builtin_isgreater((x), (y))
#define	isgreaterequal(x, y)	__builtin_isgreaterequal((x), (y))
#define	isless(x, y)		__builtin_isless((x), (y))
#define	islessequal(x, y)	__builtin_islessequal((x), (y))
#define	islessgreater(x, y)	__builtin_islessgreater((x), (y))
#define	isunordered(x, y)	__builtin_isunordered((x), (y))
#else
#define	isgreater(x, y)		(!isunordered((x), (y)) && (x) > (y))
#define	isgreaterequal(x, y)	(!isunordered((x), (y)) && (x) >= (y))
#define	isless(x, y)		(!isunordered((x), (y)) && (x) < (y))
#define	islessequal(x, y)	(!isunordered((x), (y)) && (x) <= (y))
#define	islessgreater(x, y)	(!isunordered((x), (y)) && \
					((x) > (y) || (y) > (x)))
#define	isunordered(x, y)	(isnan(x) || isnan(y))
#endif /* __MATH_BUILTIN_RELOPS */

#define	gm_signbit(x)					\
    ((sizeof (x) == sizeof (float)) ? __gm_signbitf(x)	\
    : (sizeof (x) == sizeof (double)) ? __gm_signbit(x)	\
    : __gm_signbitl(x))

typedef	__double_t	double_t;
typedef	__float_t	float_t;
#endif /* __ISO_C_VISIBLE >= 1999 */

/*
 * XOPEN/SVID
 */
#if __BSD_VISIBLE || __XSI_VISIBLE
#define	M_E		2.7182818284590452354	/* e */
#define	M_LOG2E		1.4426950408889634074	/* log 2e */
#define	M_LOG10E	0.43429448190325182765	/* log 10e */
#define	M_LN2		0.69314718055994530942	/* log e2 */
#define	M_LN10		2.30258509299404568402	/* log e10 */
#define	M_PI		3.14159265358979323846	/* pi */
#define	M_PI_2		1.57079632679489661923	/* pi/2 */
#define	M_PI_4		0.78539816339744830962	/* pi/4 */
#define	M_1_PI		0.31830988618379067154	/* 1/pi */
#define	M_2_PI		0.63661977236758134308	/* 2/pi */
#define	M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
#define	M_SQRT2		1.41421356237309504880	/* sqrt(2) */
#define	M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */

#define	MAXFLOAT	((float)3.40282346638528860e+38)
extern int signgam;
#define GM_INFINITY (__gm_infinity.__ud)
#define GM_NAN (__gm_nan.__uf)
#endif /* __BSD_VISIBLE || __XSI_VISIBLE */

#if __BSD_VISIBLE
#if 0
/* Old value from 4.4BSD-Lite math.h; this is probably better. */
#define	GM_HUGE		HUGE_VAL
#else
#define	GM_HUGE		MAXFLOAT
#endif
#endif /* __BSD_VISIBLE */

/*
 * ANSI/POSIX
 */
int	__gm_fpclassifyd(double);
int	__gm_fpclassifyf(float);
int	gm_isfinitef(float);
int	gm_isfinite(double);
int	__gm_isinff(float);
int	gm_isnormalf(float);
int gm_isnormal(double);
int	__gm_signbit(double);
int	__gm_signbitf(float);

double  gm_acos(double);
double  gm_asin(double);
double  gm_atan(double);
double  gm_atan2(double, double);
double  gm_cos(double);
double  gm_sin(double);
double  gm_tan(double);

double  gm_cosh(double);
double  gm_sinh(double);
double  gm_tanh(double);

double  gm_exp(double);
double  gm_frexp(double, int *);
double  gm_ldexp(double, int);
double  gm_log(double);
double  gm_log10(double);
double  gm_modf(double, double *);

double  gm_pow(double, double);
double  gm_sqrt(double);

double  gm_ceil(double);
double  gm_fabs(double);
double  gm_floor(double);
double  gm_fmod(double, double);

/*
 * These functions are not in C90.
 */
#if __BSD_VISIBLE || __ISO_C_VISIBLE >= 1999 || __XSI_VISIBLE
double  gm_acosh(double);
double  gm_asinh(double);
double  gm_atanh(double);
double  gm_cbrt(double);
double  gm_erf(double);
double  gm_erfc(double);
double  gm_exp2(double);
double  gm_expm1(double);
double  gm_fma(double, double, double);
double  gm_hypot(double, double);
int	gm_ilogb(double);
int	(gm_isinf)(double);
int	(gm_isnan)(double);
double  gm_lgamma(double);
long long gm_llrint(double);
long long gm_llround(double);
double  gm_log1p(double);
double  gm_logb(double);
long	gm_lrint(double);
long	gm_lround(double);
double  gm_nextafter(double, double);
double  gm_remainder(double, double);
double  gm_remquo(double, double, int *);
double  gm_rint(double);
#endif /* __BSD_VISIBLE || __ISO_C_VISIBLE >= 1999 || __XSI_VISIBLE */

#if __BSD_VISIBLE || __XSI_VISIBLE
double  gm_j0(double);
double  gm_j1(double);
double  gm_jn(int, double);
double  gm_scalb(double, double);
double  gm_y0(double);
double  gm_y1(double);
double  gm_yn(int, double);

#if __XSI_VISIBLE <= 500 || __BSD_VISIBLE
double  gm_gamma(double);
#endif
#endif /* __BSD_VISIBLE || __XSI_VISIBLE */

#if __BSD_VISIBLE || __ISO_C_VISIBLE >= 1999
double  gm_copysign(double, double);
double  gm_fdim(double, double);
double  gm_fmax(double, double);
double  gm_fmin(double, double);
double  gm_nearbyint(double);
double  gm_round(double);
double  gm_scalbln(double, long);
double  gm_scalbn(double, int);
double  gm_tgamma(double);
double  gm_trunc(double);
#endif

/*
 * BSD math library entry points
 */
#if __BSD_VISIBLE
double  gm_drem(double, double);
int	gm_finite(double);
int	gm_isnanf(float);

/*
 * Reentrant version of gamma & lgamma; passes signgam back by reference
 * as the second argument; user must allocate space for signgam.
 */
double  gm_gamma_r(double, int *);
double  gm_lgamma_r(double, int *);

/*
 * IEEE Test Vector
 */
double  gm_significand(double);
#endif /* __BSD_VISIBLE */

float  gm_acosf(float);
float  gm_asinf(float);
float  gm_atanf(float);
float  gm_atan2f(float, float);
float  gm_cosf(float);
float  gm_sinf(float);
float  gm_tanf(float);

float  gm_coshf(float);
float  gm_sinhf(float);
float  gm_tanhf(float);

float  gm_expf(float);
float  gm_frexpf(float, int *);	/* fundamentally !__pure2 */
float  gm_ldexpf(float, int);
float  gm_logf(float);
float  gm_log10f(float);
float  gm_modff(float, float *);	/* fundamentally !__pure2 */

float  gm_powf(float, float);
float  gm_sqrtf(float);

float  gm_ceilf(float);
float  gm_fabsf(float);
float  gm_floorf(float);
float  gm_fmodf(float, float);

float  gm_expm1f(float);
float  gm_scalbnf(float, int);

/* float versions of ANSI/POSIX functions */
#if __BSD_VISIBLE || __ISO_C_VISIBLE >= 1999
float  gm_roundf(float);

float  gm_erff(float);
float  gm_erfcf(float);
float  gm_hypotf(float, float);
int	gm_ilogbf(float);
float  gm_lgammaf(float);

float  gm_acoshf(float);
float  gm_asinhf(float);
float  gm_atanhf(float);
float  gm_cbrtf(float);
float  gm_exp2f(float); /*Uses double internally*/
float  gm_expm1f(float);
float  gm_logbf(float);
float  gm_copysignf(float, float);
long long gm_llrintf(float);
long long gm_llroundf(float);
long	gm_lrintf(float);
long	gm_lroundf(float);
float  gm_log1pf(float);
float  gm_nearbyintf(float);
float  gm_nextafterf(float, float);
float  gm_remainderf(float, float);
float  gm_remquof(float, float, int *);
float  gm_rintf(float);
float  gm_scalblnf(float, long);
float  gm_truncf(float);

float  gm_fdimf(float, float);
float  gm_fmaf(float, float, float); /*Uses double internally*/
float  gm_fmaxf(float, float);
float  gm_fminf(float, float);
#endif

/*
 * float versions of BSD math library entry points
 */
#if __BSD_VISIBLE
float  gm_dremf(float, float);
int	gm_finitef(float);
float  gm_gammaf(float);
float  gm_j0f(float);
float  gm_j1f(float);
float  gm_jnf(int, float);
float  gm_scalbf(float, float);
float  gm_y0f(float);
float  gm_y1f(float);
float  gm_ynf(int, float);

/*
 * Float versions of reentrant version of gamma & lgamma; passes
 * signgam back by reference as the second argument; user must
 * allocate space for signgam.
 */
float  gm_gammaf_r(float, int *);
float	gm_lgammaf_r(float, int *);

/*
 * float version of IEEE Test Vector
 */
float	gm_significandf(float);
#endif	/* __BSD_VISIBLE */

#ifdef __cplusplus
}
#endif

#endif /* !_MATH_H_ */
