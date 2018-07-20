/*
 * If type has more precision than dtype, the endpoints dtype_(min|max) are
 * of the form xxx.5; they are "out of range" because lround() rounds away
 * from 0.  On the other hand, if type has less precision than dtype, then
 * all values that are out of range are integral, so we might as well assume
 * that everything is in range.  At compile time, INRANGE(x) should reduce to
 * two floating-point comparisons in the former case, or TRUE otherwise.
 */
static const type dtype_min = DTYPE_MIN - 0.5f;
static const type dtype_max = DTYPE_MAX + 0.5f;
#define	INRANGE(x)	(dtype_max - DTYPE_MAX != 0.5f || \
			 ((x) > dtype_min && (x) < dtype_max))

dtype
fn(type x)
{

	if (INRANGE(x)) {
		x = roundit(x);
		return ((dtype)x);
	} else {
		feraiseexcept(FE_INVALID);
		return (DTYPE_MAX);
	}
}