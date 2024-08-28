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

void initPointJA2(BIGNUMPOINTJACO2* BP, const BIGNUM* x, const BIGNUM* y, const BIGNUM* z)
{
	for (int i = 0; i < x->top; i++)
	{
		BP->x.d[i] = x->d[i];
	}
	for (int i = 0; i < y->top; i++)
	{
		BP->y.d[i] = y->d[i];
	}
	for (int i = 0; i < z->top; i++)
	{
		BP->z.d[i] = z->d[i];
	}	
}