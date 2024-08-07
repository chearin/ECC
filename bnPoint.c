#include "bnPoint.h"

void initPoint(BIGNUMPOINT* BP, const BIGNUM* x, const BIGNUM* y)
{
	BP->x = x;
	BP->y = y;
}