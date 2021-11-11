/* e_rem_pio2f.c -- float version of e_rem_pio2.c
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

/* __ieee754_rem_pio2f(x,y)
 *
 * return the remainder of x rem pi/2 in y[0]+y[1]
 * use __kernel_rem_pio2f()
 */

#include "gmath.h"
#include "math_private.h"

/* Clip any extra precision in the float variable v. */
#define	cliptofloat(v)	(*(volatile float *)&(v))

/*
 * This array is like the one in e_rem_pio2.c, but the numbers are
 * single precision and the last few bits (8 here) are ignored by
 * masking them off in the float word instead of by omitting the low
 * word.
 * 
 * Masking off 8 bits is not enough, but we defer further masking to
 * runtime so that the mask is easy to change.  We now mask off 21
 * bits, which is the smallest number that makes the "quick check no
 * cancellation" detect all cancellations for cases that it is used.
 * It doesn't detect all non-cancellations, especiallly for small
 * multiples of pi/2, but then the non-quick code selects the best
 * approximation of pi/2 to use.  The result is that arg reduction is
 * always done with between 8 or 9 and 17 bits of extra precision in
 * the medium-multiple case.  With only 8 bits masked of we had
 * negative extra precision in some cases starting near +-13*pi/2.
 */
static const int32_t npio2_hw[] = {
0x3fc90f00, 0x40490f00, 0x4096cb00, 0x40c90f00, 0x40fb5300, 0x4116cb00,
0x412fed00, 0x41490f00, 0x41623100, 0x417b5300, 0x418a3a00, 0x4196cb00,
0x41a35c00, 0x41afed00, 0x41bc7e00, 0x41c90f00, 0x41d5a000, 0x41e23100,
0x41eec200, 0x41fb5300, 0x4203f200, 0x420a3a00, 0x42108300, 0x4216cb00,
0x421d1400, 0x42235c00, 0x4229a500, 0x422fed00, 0x42363600, 0x423c7e00,
0x4242c700, 0x42490f00
};

/*
 * invpio2:  24 bits of 2/pi
 * pio2_1:   first  17 bit of pi/2
 * pio2_1t:  pi/2 - pio2_1
 * pio2_2:   second 17 bit of pi/2
 * pio2_2t:  pi/2 - (pio2_1+pio2_2)
 * pio2_3:   third  17 bit of pi/2
 * pio2_3t:  pi/2 - (pio2_1+pio2_2+pio2_3)
 */

static const float
zero =  0.0000000000e+00f, /* 0x00000000 */
half =  5.0000000000e-01f, /* 0x3f000000 */
two8 =  2.5600000000e+02f, /* 0x43800000 */
invpio2 =  6.3661980629e-01f, /* 0x3f22f984 */
pio2_1  =  1.5707855225e+00f, /* 0x3fc90f80 */
pio2_1t =  1.0804334124e-05f, /* 0x37354443 */
pio2_2  =  1.0804273188e-05f, /* 0x37354400 */
pio2_2t =  6.0770999344e-11f, /* 0x2e85a308 */
pio2_3  =  6.0770943833e-11f, /* 0x2e85a300 */
pio2_3t =  6.1232342629e-17f; /* 0x248d3132 */

#ifdef INLINE_REM_PIO2
#ifdef _MSC_VER
static __forceinline
#else
static __inline __attribute__((always_inline))
#endif
#endif
int32_t __ieee754_rem_pio2f(float x, float *y)
{
	float z,w,t,r,fn;
	float tx[3];
	int32_t e0,i,j,nx,n,ix,hx;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix<=0x3f490fd8)   /* |x| ~<= pi/4 , no need for reduction */
	    {y[0] = x; y[1] = 0; return 0;}
	if(ix<0x4016cbe4) {  /* |x| < 3pi/4, special case with n=+-1 */
	    if(hx>0) {
		z = x - pio2_1;
		if((ix&0xfffe0000)!=0x3fc80000) { /* 17+24 bit pi OK */
		    y[0] = z - pio2_1t;
		    y[1] = (z-cliptofloat(y[0]))-pio2_1t;
		} else {		/* near pi/2, use 17+17+24 bit pi */
		    z -= pio2_2;
		    y[0] = z - pio2_2t;
		    y[1] = (z-cliptofloat(y[0]))-pio2_2t;
		}
		return 1;
	    } else {	/* negative x */
		z = x + pio2_1;
		if((ix&0xfffe0000)!=0x3fc80000) { /* 17+24 bit pi OK */
		    y[0] = z + pio2_1t;
		    y[1] = (z-cliptofloat(y[0]))+pio2_1t;
		} else {		/* near pi/2, use 17+17+24 bit pi */
		    z += pio2_2;
		    y[0] = z + pio2_2t;
		    y[1] = (z-cliptofloat(y[0]))+pio2_2t;
		}
		return -1;
	    }
	}
	if(ix<=0x43490f80) { /* |x| ~<= 2^7*(pi/2), medium size */
	    t  = gm_fabsf(x);
	    n  = (int32_t) (t*invpio2+half);
	    fn = (float)n;
	    r  = t-fn*pio2_1;
	    w  = fn*pio2_1t;	/* 1st round good to 40 bit */
	    if(n<32&&(ix&0xffe00000)!=(npio2_hw[n-1]&0xffe00000)) {
		y[0] = r-w;	/* quick check no cancellation */
	    } else {
	        uint32_t high;
	        j  = ix>>23;
	        y[0] = r-w;
		GET_FLOAT_WORD(high,y[0]);
	        i = j-((high>>23)&0xff);
	        if(i>8) {  /* 2nd iteration needed, good to 57 */
		    t  = r;
		    w  = fn*pio2_2;
		    r  = t-w;
		    w  = fn*pio2_2t-((t-r)-w);
		    y[0] = r-w;
		    GET_FLOAT_WORD(high,y[0]);
		    i = j-((high>>23)&0xff);
		    if(i>25)  {	/* 3rd iteration need, 74 bits acc */
		    	t  = r;	/* will cover all possible cases */
		    	w  = fn*pio2_3;
		    	r  = t-w;
		    	w  = fn*pio2_3t-((t-r)-w);
		    	y[0] = r-w;
		    }
		}
	    }
	    y[1] = (r-cliptofloat(y[0]))-w;
	    if(hx<0) 	{y[0] = -y[0]; y[1] = -y[1]; return -n;}
	    else	 return n;
	}
    /*
     * all other (large) arguments
     */
	if(ix>=0x7f800000) {		/* x is inf or NaN */
	    y[0]=y[1]=x-x; return 0;
	}
    /* set z = scalbn(|x|,ilogb(x)-7) */
	e0 	= (ix>>23)-134;		/* e0 = ilogb(z)-7; */
	SET_FLOAT_WORD(z, ix - ((int32_t)(e0<<23)));
	for(i=0;i<2;i++) {
		tx[i] = (float)((int32_t)(z));
		z     = (z-tx[i])*two8;
	}
	tx[2] = z;
	nx = 3;
	while(tx[nx-1]==zero) nx--;	/* skip zero term */
	n  =  __kernel_rem_pio2f(tx,y,e0,nx,2);
	if(hx<0) {y[0] = -y[0]; y[1] = -y[1]; return -n;}
	return n;
}
