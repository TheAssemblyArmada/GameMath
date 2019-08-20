/*
 * drem() wrapper for remainder().
 *
 * Written by J.T. Conklin, <jtc@wimsey.com>
 * Placed into the Public Domain, 1994.
 */

#include "gmath.h"

double
gm_drem(x, y)
	double x, y;
{
	return gm_remainder(x, y);
}
