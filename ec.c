#include "ec.h"

void initEcurve(ECURVE* EC, const BIGNUM* a, const BIGNUM* b)
{
	EC->a = a;
	EC->b = b;
}