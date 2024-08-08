#pragma once
#include "bn.h"
#include "bnPoint.h"
#include "ec.h"
#include "mul.h"
#include "reduction.h"
#include "inversion.h"

void ECADDJ(BIGNUMPOINTJACO* r, const BIGNUMPOINTJACO* p1, const BIGNUMPOINT* p2, const BIGNUM* P);
void ECDBLJ(BIGNUMPOINTJACO* r, const BIGNUMPOINTJACO* p1, const BIGNUM* P);
void J2A(BIGNUMPOINT* r, BIGNUMPOINTJACO* a);