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

#ifndef _MATH_PRIVATE_H_
#define	_MATH_PRIVATE_H_

#include <stdint.h>
#include <sys/types.h>
#include <float.h>

#if defined(__linux__)
#include <endian.h>
#elif defined(__APPLE__) && defined(__MACH__)
#include	<libkern/OSByteOrder.h>
#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__OpenBSD__)
#include	<sys/endian.h>
#endif

/*
 * The original fdlibm code used statements like:
 *	n0 = ((*(int*)&one)>>29)^1;		* index of high word *
 *	ix0 = *(n0+(int*)&x);			* high word of x *
 *	ix1 = *((1-n0)+(int*)&x);		* low word of x *
 * to dig two 32 bit words out of the 64 bit IEEE floating point
 * value.  That is non-ANSI, and, moreover, the gcc instruction
 * scheduler gets it wrong.  We instead use the following macros.
 * Unlike the original code, we determine the endianness at compile
 * time, not at run time; I don't see much benefit to selecting
 * endianness at run time.
 */

/*
 * A union which permits us to convert between a double and two 32 bit
 * ints.
 */
#if defined(_WIN32) || __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

typedef union
{
    double value;
    struct
    {
        uint32_t lsw;
        uint32_t msw;
    } parts;
    struct
    {
        uint64_t w;
    } xparts;
} ieee_double_shape_type;

union IEEEf2bits {
    float	f;
    struct {
        unsigned int	man : 23;
        unsigned int	exp : 8;
        unsigned int	sign : 1;
    } bits;
};

union IEEEd2bits {
    double	d;
    struct {
        unsigned int	manl : 32;
        unsigned int	manh : 20;
        unsigned int	exp  : 11;
        unsigned int	sign : 1;
    } bits;
};

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

typedef union
{
  double value;
  struct
  {
    uint32_t msw;
    uint32_t lsw;
  } parts;
  struct
  {
      uint64_t w;
  } xparts;
} ieee_double_shape_type;

union IEEEf2bits {
	float	f;
	struct {
		unsigned int	sign:1;
		unsigned int	exp	:8;
		unsigned int	man	:23;
	} bits;
};

union IEEEd2bits {
    double	d;
    struct {
        unsigned int	sign : 1;
        unsigned int	exp  : 11;
        unsigned int	manh : 20;
        unsigned int	manl : 32;
    } bits;
};
#endif

/* Get two 32 bit ints from a double.  */

#define EXTRACT_WORDS(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)

/* Get a 64-bit int from a double. */
#define EXTRACT_WORD64(ix,d)					\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix) = ew_u.xparts.w;						\
} while (0)

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i,d)					\
do {								\
  ieee_double_shape_type gh_u;					\
  gh_u.value = (d);						\
  (i) = gh_u.parts.msw;						\
} while (0)

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i,d)					\
do {								\
  ieee_double_shape_type gl_u;					\
  gl_u.value = (d);						\
  (i) = gl_u.parts.lsw;						\
} while (0)

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d,ix0,ix1)					\
do {								\
  ieee_double_shape_type iw_u;					\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)

/* Set a double from a 64-bit int. */
#define INSERT_WORD64(d,ix)					\
do {								\
  ieee_double_shape_type iw_u;					\
  iw_u.xparts.w = (ix);						\
  (d) = iw_u.value;						\
} while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)					\
do {								\
  ieee_double_shape_type sh_u;					\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d,v)					\
do {								\
  ieee_double_shape_type sl_u;					\
  sl_u.value = (d);						\
  sl_u.parts.lsw = (v);						\
  (d) = sl_u.value;						\
} while (0)

/*
 * A union which permits us to convert between a float and a 32 bit
 * int.
 */
#if defined(__GNUC__) || defined(__clang__)
/*
 * GCC/Clang specific syntax to allow violation of strict aliasing rules.
 */
typedef union
{
  float __attribute__((__may_alias__)) value;
  uint32_t __attribute__((__may_alias__)) word;
} ieee_float_shape_type;
#else
typedef union
{
  float value;
  uint32_t word;
} ieee_float_shape_type;
#endif

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)					\
do {								\
  ieee_float_shape_type gf_u;					\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)					\
do {								\
  ieee_float_shape_type sf_u;					\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

#ifdef FLT_EVAL_METHOD
/*
 * Attempt to get strict C99 semantics for assignment with non-C99 compilers.
 */
#if FLT_EVAL_METHOD == 0 || __GNUC__ == 0
#define	STRICT_ASSIGN(type, lval, rval)	((lval) = (rval))
#else
#define	STRICT_ASSIGN(type, lval, rval) do {	\
	volatile type __lval;			\
						\
	if (sizeof(type) >= sizeof(long double))	\
		(lval) = (rval);		\
	else {					\
		__lval = (rval);		\
		(lval) = __lval;		\
	}					\
} while (0)
#endif
#endif /* FLT_EVAL_METHOD */

 /*
  * The rnint() family rounds to the nearest integer for a restricted range
  * range of args (up to about 2**MANT_DIG).  We assume that the current
  * rounding mode is FE_TONEAREST so that this can be done efficiently.
  * Extra precision causes more problems in practice, and we only centralize
  * this here to reduce those problems, and have not solved the efficiency
  * problems.  The exp2() family uses a more delicate version of this that
  * requires extracting bits from the intermediate value, so it is not
  * centralized here and should copy any solution of the efficiency problems.
  */

static inline double
rnint(double x)
{
	/*
	 * This casts to double to kill any extra precision.  This depends
	 * on the cast being applied to a double_t to avoid compiler bugs
	 * (this is a cleaner version of STRICT_ASSIGN()).  This is
	 * inefficient if there actually is extra precision, but is hard
	 * to improve on.  We use double_t in the API to minimise conversions
	 * for just calling here.  Note that we cannot easily change the
	 * magic number to the one that works directly with double_t, since
	 * the rounding precision is variable at runtime on x86 so the
	 * magic number would need to be variable.  Assuming that the
	 * rounding precision is always the default is too fragile.  This
	 * and many other complications will move when the default is
	 * changed to FP_PE.
	 */
	return ((double)(x + 0x1.8p52) - 0x1.8p52);
}

static inline float
rnintf(float x)
{
	/*
	 * As for rnint(), except we could just call that to handle the
	 * extra precision case, usually without losing efficiency.
	 */
	return ((float)(x + 0x1.8p23F) - 0x1.8p23F);
}

#define	irint(x) ((int)(x))

/*
 * ieee style elementary functions
 *
 * We rename functions here to improve other sources' diffability
 * against fdlibm.
 */
#define	__ieee754_sqrt	gm_sqrt
#define	__ieee754_acos	gm_acos
#define	__ieee754_acosh	gm_acosh
#define	__ieee754_log	gm_log
#define	__ieee754_atanh	gm_atanh
#define	__ieee754_asin	gm_asin
#define	__ieee754_atan2	gm_atan2
#define	__ieee754_exp	gm_exp
#define	__ieee754_cosh	gm_cosh
#define	__ieee754_fmod	gm_fmod
#define	__ieee754_pow	gm_pow
#define	__ieee754_lgamma gm_lgamma
#define	__ieee754_gamma	gm_gamma
#define	__ieee754_lgamma_r gm_lgamma_r
#define	__ieee754_gamma_r gm_gamma_r
#define	__ieee754_log10	gm_log10
#define	__ieee754_log2	gm_log2
#define	__ieee754_sinh	gm_sinh
#define	__ieee754_hypot	gm_hypot
#define	__ieee754_j0	gm_j0
#define	__ieee754_j1	gm_j1
#define	__ieee754_y0	gm_y0
#define	__ieee754_y1	gm_y1
#define	__ieee754_jn	gm_jn
#define	__ieee754_yn	gm_yn
#define	__ieee754_remainder gm_remainder
#define	__ieee754_scalb	gm_scalb
#define	__ieee754_sqrtf	gm_sqrtf
#define	__ieee754_acosf	gm_acosf
#define	__ieee754_acoshf gm_acoshf
#define	__ieee754_logf	gm_logf
#define	__ieee754_atanhf gm_atanhf
#define	__ieee754_asinf	gm_asinf
#define	__ieee754_atan2f gm_atan2f
#define	__ieee754_expf	gm_expf
#define	__ieee754_coshf	gm_coshf
#define	__ieee754_fmodf	gm_fmodf
#define	__ieee754_powf	gm_powf
#define	__ieee754_lgammaf gm_lgammaf
#define	__ieee754_gammaf gm_gammaf
#define	__ieee754_lgammaf_r gm_lgammaf_r
#define	__ieee754_gammaf_r gm_gammaf_r
#define	__ieee754_log10f gm_log10f
#define	__ieee754_log2f gm_log2f
#define	__ieee754_sinhf	gm_sinhf
#define	__ieee754_hypotf gm_hypotf
#define	__ieee754_j0f	gm_j0f
#define	__ieee754_j1f	gm_j1f
#define	__ieee754_y0f	gm_y0f
#define	__ieee754_y1f	gm_y1f
#define	__ieee754_jnf	gm_jnf
#define	__ieee754_ynf	gm_ynf
#define	__ieee754_remainderf gm_remainderf
#define	__ieee754_scalbf gm_scalbf

/* fdlibm kernel function */
int	__ieee754_rem_pio2(double,double*);
double	__kernel_sin(double,double,int);
double	__kernel_cos(double,double);
double	__kernel_tan(double,double,int);
int	__kernel_rem_pio2(double*,double*,int,int,int);
double __ldexp_exp(double, int);

/* float versions of fdlibm kernel functions */
int	__ieee754_rem_pio2f(float,float*);
float	__kernel_sinf(float,float,int);
float	__kernel_cosf(float,float);
float	__kernel_tanf(float,float,int);
int	__kernel_rem_pio2f(float*,float*,int,int,int);
float __ldexp_expf(float, int);

#endif /* !_MATH_PRIVATE_H_ */
