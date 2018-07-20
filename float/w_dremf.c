/*
 * dremf() wrapper for remainderf().
 *
 * Written by J.T. Conklin, <jtc@wimsey.com>
 * Placed into the Public Domain, 1994.
 */
/* $FreeBSD$ */

#include "gmath.h"
#include "math_private.h"

float
gm_dremf(float x, float y)
{
	return gm_remainderf(x, y);
}
