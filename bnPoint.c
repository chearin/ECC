#include "bnPoint.h"

void initPoint(BIGNUMPOINT* BP, const BIGNUM* x, const BIGNUM* y)
{
	BP->x = x;
	BP->y = y;
}

void initPointJA(BIGNUMPOINTJACO* BP, const BIGNUM* x, const BIGNUM* y, const BIGNUM* z)
{
	BP->x = x;
	BP->y = y;
	BP->z = z;
}