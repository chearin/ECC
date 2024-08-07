#pragma once
#include "bn.h"
#include "bnPoint.h"
#include "ec.h"
#include "mul.h"
#include "reduction.h"
#include "inversion.h"

void ECADD(BIGNUMPOINT* r, const BIGNUMPOINT* p1, const BIGNUMPOINT* p2, const BIGNUM* P);
void ECDBL(BIGNUMPOINT* r, const ECURVE* e, const BIGNUMPOINT* p1, const BIGNUM* P);